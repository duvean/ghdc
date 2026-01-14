#include "semantic_analyzer.h"
#include <iostream>

void SemanticAnalyzer::analyze(ProgramNode* root) {
    if (!root) return;
    // В ProgramNode у тебя вектор decls
    for (auto decl : root->decls) {
        analyzeDecl(decl);
    }
}

void SemanticAnalyzer::analyzeDeclList(DeclListNode* list) {
    if (!list) return;
    for (auto decl : list->decls) {
        analyzeDecl(decl);
    }
}

void SemanticAnalyzer::analyzeDecl(DeclNode* node) {
    if (!node) return;

    // 1. Обработка переменных
    if (node->type == DECL_VAR) {
        if (node->expr) {
            analyzeExpr(node->expr);

            // 2. Регистрируем переменную
            LocalVariable varInfo;
            varInfo.index = nextLocalIndex++;
            varInfo.type = node->expr->inferredType; // Теперь это копирование указателя (SemanticType*)

            symbolTable[node->name] = varInfo;

            // 3. Атрибутируем узел
            node->localVarIndex = varInfo.index;
            node->inferredType = SemanticType::Void(); // Фабричный метод

            std::cout << "[Semantic] Declared var '" << node->name
                << "' index=" << varInfo.index
                << " type=" << (varInfo.type ? varInfo.type->getDescriptor() : "?") << "\n";
        }
    }
    // 2. Обработка заголовков функций
    else if (node->type == DECL_FUNC_SIGN) {
        FunctionSignature sig;
        std::vector<SemanticType*> allTypes; // Вектор новых типов
        collectTypes(node->typeExpr, allTypes); // Используем обновленный collectTypes

        if (!allTypes.empty()) {
            sig.returnType = allTypes.back();
            allTypes.pop_back();
            sig.paramTypes = allTypes;
            functionSignatures[node->name] = sig;
            std::cout << "[Semantic] Registered signature for '" << node->name << "'\n";
        }
    }

    // 3. Обработка тел функций
    else if (node->type == DECL_FUNC) {
        symbolTable.clear();
        nextLocalIndex = 0;

        auto it = functionSignatures.find(node->name);
        if (it == functionSignatures.end()) {
            std::cerr << "[Error] No signature for function: " << node->name << "\n";
            return;
        }
        FunctionSignature& sig = it->second;

        // Параметры
        std::vector<DeclNode*> parameters;
        if (node->paramsList) parameters = dynamic_cast<DeclListNode*>(node->paramsList)->decls;

        for (size_t i = 0; i < parameters.size(); ++i) {
            // Передаем SemanticType* в analyzePattern
            if (i < sig.paramTypes.size()) {
                analyzePattern(parameters[i]->expr, sig.paramTypes[i], nextLocalIndex++);
            }
        }

        // Обработка Where-блока (если есть)
        if (node->whereBlock) {
            DeclListNode* wList = dynamic_cast<DeclListNode*>(node->whereBlock);
            if (wList) analyzeDeclList(wList);
        }

        if (node->expr) {
            analyzeExpr(node->expr);

            SemanticType* actualType = node->expr->inferredType;
            SemanticType* expectedType = sig.returnType;

            // Сравнение типов через equals (так как это объекты)
            if (actualType->kind == TypeKind::LIST &&
                actualType->subType->kind == TypeKind::UNKNOWN) {

                if (expectedType->kind == TypeKind::LIST) {
                    // "Кастим" пустой список к ожидаемому типу
                    node->expr->inferredType = expectedType;
                    actualType = expectedType; // Обновляем локальную переменную для проверки ниже
                    std::cout << "[Semantic] Implicitly typed empty list [] to " << expectedType->getDescriptor() << "\n";
                }
            }

            if (!actualType->equals(expectedType)) {
                std::cout << "[Warning] Return type mismatch in '" << node->name
                    << "'. Expected " << expectedType->getDescriptor()
                    << ", got " << actualType->getDescriptor() << "\n";
            }
        }

        // Регистрация в Constant Pool
        std::string desc = "(";
        for (auto* t : sig.paramTypes) desc += t->getDescriptor(); // Метод класса SemanticType
        desc += ")" + sig.returnType->getDescriptor();

        node->constPoolIndex = constPool.addUtf8(node->name);
        constPool.addUtf8(desc);
    }

    // 4. Обработка Let-блоков
    else if (node->type == DECL_BLOCK) {
        DeclListNode* list = dynamic_cast<DeclListNode*>(node->letBlock);
        if (list) analyzeDeclList(list);
    }
}

void SemanticAnalyzer::analyzeExpr(ExprNode* node) {
    if (!node) return;

    switch (node->type) {
    case EXPR_LITERAL: {
        if (node->value.find('.') != std::string::npos) {
            node->inferredType = SemanticType::Float();
            float val = std::stof(node->value);
            node->constPoolIndex = constPool.addFloat(val);
        }
        else if (isdigit(node->value[0]) || node->value[0] == '-') {
            node->inferredType = SemanticType::Int();
            int val = std::stoi(node->value);
            node->constPoolIndex = constPool.addInteger(val);
        }
        else {
            node->inferredType = SemanticType::String();
            node->constPoolIndex = constPool.addStringLiteral(node->value);
        }
        break;
    }

    case EXPR_VAR:
    case EXPR_PATTERN_VAR: {
        if (symbolTable.count(node->name)) {
            auto& info = symbolTable[node->name];
            node->inferredType = info.type; // SemanticType*
            node->localVarIndex = info.index;
            node->isFunctionRef = false; // Это переменная

            // Debug print
            // std::cout << "Linked " << node->name << " type: " << info.type->getDescriptor() << "\n";
        }
        else {
            // Проверяем глобальные функции
            if (functionSignatures.count(node->name)) {
                node->isFunctionRef = true; // Это функция!
                node->localVarIndex = -1;
                node->inferredType = functionSignatures[node->name].returnType;
            }
            else {
                std::cerr << "[Semantic Error] Undefined identifier: " << node->name << "\n";
                node->inferredType = SemanticType::Unknown();
            }
        }
        break;
    }

    case EXPR_BINARY: {
        analyzeExpr(node->left);
        analyzeExpr(node->right);

        SemanticType* lType = node->left->inferredType;
        SemanticType* rType = node->right->inferredType;

        if (!lType || !rType) return; // Защита от null

        // --- Оператор CONS (:) ---
        if (node->op == ":") {
            // Результат - всегда список типа головы
            node->inferredType = SemanticType::List(lType);
            // Тут можно добавить проверку: rType должен быть List(lType)
        }
        // --- Сравнения ---
        else if (node->op == "<=" || node->op == "==" || node->op == ">" || node->op == "<") {
            node->inferredType = SemanticType::Bool();

            // Авто-каст для примитивов (Int vs Float)
            if (lType->base == BaseType::FLOAT || rType->base == BaseType::FLOAT) {
                if (lType->base == BaseType::INT) {
                    node->left = createCastNode(node->left, SemanticType::Float());
                }
                if (rType->base == BaseType::INT) {
                    node->right = createCastNode(node->right, SemanticType::Float());
                }
            }
        }
        // --- Арифметика ---
        else {
            // Проверяем BaseType, так как теперь это структура
            bool leftIsFloat = (lType->kind == TypeKind::PRIMITIVE && lType->base == BaseType::FLOAT);
            bool rightIsFloat = (rType->kind == TypeKind::PRIMITIVE && rType->base == BaseType::FLOAT);

            if (leftIsFloat || rightIsFloat) {
                node->inferredType = SemanticType::Float();

                if (lType->base == BaseType::INT) {
                    node->left = createCastNode(node->left, SemanticType::Float());
                    std::cout << "[Semantic] Auto-cast LEFT to Float\n";
                }
                if (rType->base == BaseType::INT) {
                    node->right = createCastNode(node->right, SemanticType::Float());
                    std::cout << "[Semantic] Auto-cast RIGHT to Float\n";
                }
            }
            else {
                node->inferredType = SemanticType::Int();
            }
        }
        break;
    }

    case EXPR_TYPE_LIST:
        // Это декларация типа, например [Int]
        // Здесь мы не знаем тип элементов без collectTypes, пока оставим List(Unknown)
        node->inferredType = SemanticType::List(SemanticType::Unknown());
        break;

    case EXPR_LIST:
    case EXPR_ARRAY: {
        SemanticType* elemType = SemanticType::Unknown();

        // Если массив не пустой, берем тип первого элемента
        if (!node->block.empty()) {
            analyzeExpr(node->block[0]);
            elemType = node->block[0]->inferredType;

            // Анализируем остальные
            for (size_t i = 1; i < node->block.size(); ++i) {
                analyzeExpr(node->block[i]);
                // Тут можно проверить, что типы совпадают
            }
        }
        // Для конструкции [x] (где x в node->left/branch)
        else if (node->left) {
            analyzeExpr(node->left);
            elemType = node->left->inferredType;
        }

        node->inferredType = SemanticType::List(elemType);
        break;
    }

    case EXPR_IF:
        analyzeExpr(node->cond);
        analyzeExpr(node->expr_true);
        analyzeExpr(node->expr_false);
        node->inferredType = node->expr_true->inferredType;
        break;

    case EXPR_FUNC_CALL: {
        if (node->function) analyzeExpr(node->function);
        for (ExprNode* arg : node->arguments) analyzeExpr(arg);
        if (node->left) analyzeExpr(node->left);
        if (node->right) analyzeExpr(node->right);

        std::string pureName = node->name;
        size_t spacePos = pureName.find(' ');
        if (spacePos != std::string::npos) pureName = pureName.substr(0, spacePos);

        if (functionSignatures.count(pureName)) {
            node->inferredType = functionSignatures[pureName].returnType;
        }
        else {
            // Если имя не найдено, возможно это вызов через переменную-функцию (Higher Order Function)
            // Но пока ставим Unknown
            node->inferredType = SemanticType::Unknown();
        }
        break;
    }

    case EXPR_CASTING:
        // Cast node уже имеет явно заданный тип при создании
        // (см. createCastNode)
        break;

    default:
        if (node->left) analyzeExpr(node->left);
        if (node->right) analyzeExpr(node->right);
        break;
    }
}

void SemanticAnalyzer::analyzePattern(ExprNode* pattern, SemanticType* expectedType, int sourceLocalIndex) {
    if (!pattern || !expectedType) return;

    if (pattern->type == EXPR_PATTERN_CONS) {
        if (expectedType->kind == TypeKind::LIST) {
            // Голова списка (y) имеет тип подтипа (если [T], то y - это T)
            SemanticType* headType = expectedType->subType;

            if (pattern->left) {
                symbolTable[pattern->left->name] = { nextLocalIndex++, headType };
            }
            // Хвост списка (ys) - это тот же список [T]
            if (pattern->right) {
                symbolTable[pattern->right->name] = { nextLocalIndex++, expectedType };
            }
        }
    }
    else if (pattern->type == EXPR_PATTERN_VAR) {
        symbolTable[pattern->name] = { sourceLocalIndex, expectedType };
    }
}

std::string SemanticAnalyzer::makeMethodDescriptor(DeclNode* funcNode) {
    // Пока упрощенно: считаем, что аргументов нет (нужно доработать парсинг аргументов)
    // И возвращаем тип, который мы вывели (inferredType)
    std::string retDesc = funcNode->inferredType->getDescriptor();
    return "()" + retDesc;
}

void SemanticAnalyzer::collectTypes(ASTNode* node, std::vector<SemanticType*>& types) {
    if (!node) return;
    ExprNode* expr = dynamic_cast<ExprNode*>(node);
    if (!expr) return;

    if (expr->type == EXPR_TYPE_FUNCTION) {
        // Узел вида: ArgType -> ReturnType (каррирование)
        collectTypes(expr->left, types);
        collectTypes(expr->right, types);
    }
    else if (expr->type == EXPR_TYPE_PRIMITIVE) {
        // ВАЖНО: Добавлены скобки () для вызова статических методов
        if (expr->value == "Int") types.push_back(SemanticType::Int());
        else if (expr->value == "Float") types.push_back(SemanticType::Float());
        else if (expr->value == "Bool") types.push_back(SemanticType::Bool());
        else if (expr->value == "String") types.push_back(SemanticType::String());
        else types.push_back(SemanticType::Unknown());
    }
    else if (expr->type == EXPR_TYPE_LIST) {
        // Логика: Тип списка [T] лежит в expr->right
        // Нам нужно узнать тип T, чтобы создать List(T)

        std::vector<SemanticType*> innerTypes;
        // Рекурсивно узнаем, что внутри списка (там может быть Int, Float или еще один List)
        collectTypes(expr->right, innerTypes);

        if (!innerTypes.empty()) {
            // Берем последний найденный тип (обычно он там один)
            SemanticType* inner = innerTypes.back();
            // Создаем обертку List
            types.push_back(SemanticType::List(inner));
        }
        else {
            // Если внутри пусто или ошибка, создаем List(Unknown)
            types.push_back(SemanticType::List(SemanticType::Unknown()));
        }
    }
}

// Создание узла кастинга. Оборачиваем старый узел в новый.
ExprNode* SemanticAnalyzer::createCastNode(ExprNode* target, SemanticType* toType) {
    ExprNode* castNode = new ExprNode(NodeType::EXPR_CASTING);
    castNode->left = target;
    castNode->inferredType = toType; // Просто копируем указатель
    castNode->isCastNode = true;
    return castNode;
}
