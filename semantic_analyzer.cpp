#include <iostream>
#include "class_builder.h"
#include "code_generator.h"
#include "semantic_analyzer.h"

void SemanticAnalyzer::analyze(ProgramNode* root) {
    analyzeProgram(root);
}

void SemanticAnalyzer::analyzeProgram(ProgramNode* root) {
    if (!root) return;

    // 1. СНАЧАЛА создаем класс
    if (currentClass == nullptr) {
        constPool.addUtf8("Code");
        currentClass = new JvmClass("HaskellProgram", constPool);
        currentClass->classIdx = constPool.addClass("HaskellProgram");
        currentClass->superIdx = constPool.addClass("java/lang/Object");

        // 1. Создаем MethodRef на Object.<init> (инструкция invokespecial будет использовать это)
        // Это индекс #9 в вашем выводе
        int superInitRef = constPool.addMethodRef("java/lang/Object", "<init>", "()V");

        // 2. Создаем структуру метода для НАШЕГО класса
        JvmMethod initMethod("<init>", "()V", nullptr);
        initMethod.nameIdx = constPool.addUtf8("<init>");
        initMethod.descIdx = constPool.addUtf8("()V");
        initMethod.accessFlags = 0x0001; // ACC_PUBLIC (не static!)

        currentClass->methods.push_back(initMethod);
    }

    // 2. Сначала собираем ВСЕ сигнатуры и перечисления
    // Это важно, чтобы функции видели друг друга
    for (auto* decl : root->decls) {
        if (decl && (decl->type == DECL_FUNC_SIGN || decl->type == DECL_DATA)) {
            analyzeDecl(decl);
        }
    }

    // 3. Затем анализируем тела функций (второй проход)
    for (auto* decl : root->decls) {
        if (decl && (decl->type == DECL_FUNC || decl->type == DECL_VAR)) {
            analyzeDecl(decl);
        }
    }

    // 4. После анализа всех деклараций добавляем JVM Entry Point
    // Это public static void main(String[] args)

    // Регистрируем дескриптор
    std::string mainDesc = "([Ljava/lang/String;)V";

    // Регистрируем MethodRef для самой точки входа
    int javaMainRef = constPool.addMethodRef("HaskellProgram", "main", mainDesc);

    JvmMethod entryPoint("main", mainDesc, nullptr);
    entryPoint.nameIdx = constPool.addUtf8("main");
    entryPoint.descIdx = constPool.addUtf8(mainDesc);
    entryPoint.accessFlags = 0x0009; // ACC_PUBLIC | ACC_STATIC

    currentClass->methods.push_back(entryPoint);
    std::cout << "[JvmGen] Added synthetic JVM entry point 'main' -> calls 'haskellMain'\n";

    // Генерация байткода
    for (auto& method : currentClass->methods) {
        if (method.name == "<init>") continue; // Конструктор пишем вручную или отдельно
        
        CodeGenerator gen(constPool, method.bytecode);
        gen.generate(method);
    }

    ClassBuilder builder("output/HaskellProgram.class", currentClass);
    bool res = builder.build();
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

            // 1. Берем текущий свободный индекс
            int index = nextLocalIndex++; 
            SemanticType* type = node->expr->inferredType;

            // 2. ОБЯЗАТЕЛЬНО сохраняем индекс в сам узел декларации
            // Чтобы CodeGenerator знал, куда делать ISTORE/ASTORE
            node->localVarIndex = index; 
            node->inferredType = type;

            // 3. Регистрируем в таблице символов для последующих вызовов
            // Используем ТОТ ЖЕ самый индекс
            symbolTable[node->name] = LocalVariable(type, index);

            std::cout << "[Semantic] Declared var '" << node->name
                    << "' index=" << index
                    << " type=" << (type ? type->getDescriptor() : "?") << "\n";
        }
    }
    
    // 2. Обработка заголовков функций
    else if (node->type == DECL_FUNC_SIGN) {
        std::vector<SemanticType*> allTypes;
        collectTypes(node->typeExpr, allTypes);

        if (!allTypes.empty()) {
            FunctionSignature sig;
            sig.returnType = allTypes.back(); // Последний - всегда возвращаемое значение
            allTypes.pop_back();
            sig.paramTypes = allTypes;        // Все остальные - аргументы
            functionSignatures[node->name] = sig;

            std::cout << "[Semantic] Registered signature for '" << node->name 
                    << "' (" << sig.paramTypes.size() << " params)\n";
        }
    }

    // 3. Обработка тел функций
    else if (node->type == DECL_FUNC) {
        // 1. Ищем сигнатуру
        auto it = functionSignatures.find(node->name);
        if (it == functionSignatures.end()) {
            std::cerr << "[Error] No signature for function: " << node->name << "\n";
            return;
        }
        FunctionSignature& sig = it->second;
        std::string internalName = node->name;
        std::string descriptor = sig.getDescriptor();

        // Обработка main
        if (node->name == "main") {
            internalName = "haskellMain";
            descriptor = "()V"; 
            
            // Создаем MethodRef для Haskell-кода
            // Это позволит JVM найти этот метод внутри этого же класса (#3)
            int haskellMainRef = constPool.addMethodRef("HaskellProgram", internalName, descriptor);
            
            std::cout << "[JvmGen] MethodRef for haskellMain created at index: " << haskellMainRef << "\n";
        }

        // 2. Создаем JVM метод
        JvmMethod method(internalName, descriptor, node);
        
        // Регистрируем имя и дескриптор в Constant Pool
        method.nameIdx = constPool.addUtf8(internalName);
        method.descIdx = constPool.addUtf8(descriptor);

        // 3. Подготовка к анализу тела
        // Очищаем всё что не является глобальными константами
        for (auto it = symbolTable.begin(); it != symbolTable.end(); ) {
            if (!it->second.isEnum) { // Если это обычная переменная из прошлой функции
                it = symbolTable.erase(it);
            } else {
                ++it;
            }
        }

        // Сначала регистрируем сами аргументы в таблице символов
        // Чтобы к ним можно было обращаться по именам, если это просто переменные
        // localIdx для новых переменных в паттернах начнется ПОСЛЕ аргументов
        int numArgs = (int)sig.paramTypes.size();
        int nextFreeSlot = numArgs;

        // 4. Анализ аргументов (Паттерны)
        if (node->paramsList) {
            auto* declList = dynamic_cast<DeclListNode*>(node->paramsList);
            for (size_t i = 0; i < declList->decls.size(); ++i) {
                // i - индекс аргумента
                // nextFreeSlot - это куда будут выгружаться части паттерна (x, xs)
                analyzePattern(declList->decls[i]->expr, sig.paramTypes[i], nextFreeSlot, (int)i);
            }
        }

        // 5. Анализ тела функции
        if (node->expr) {
            // Пробрасываем ожидаемый тип возврата в выражение тела
            analyzeExpr(node->expr, sig.returnType); 

            // Теперь этот костыль ниже не обязателен, 
            // так как EXPR_ARRAY уже сам всё подхватит,
            // пока оставлю
            // if (node->expr->type == EXPR_ARRAY && node->expr->block.empty()) {
            //     node->expr->inferredType = sig.returnType;
            // }
        }

        // 6. ФИНАЛ: Сохраняем локальные переменные в структуру метода
        // (SymbolTable сейчас содержит все переменные этой функции)
        for (std::map<std::string, LocalVariable>::const_iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
            // it->first — имя переменной (string)
            // it->second — объект LocalVariable
            method.locals.push_back(it->second);
        }
        
        if (currentClass) {
            auto it = std::find_if(currentClass->methods.begin(), currentClass->methods.end(),
            [&](const JvmMethod& m) { return m.name == internalName; });

            if (it != currentClass->methods.end()) {
                it->bodies.push_back(node); // Добавляем еще одно уравнение к существующему методу
            } else {
                currentClass->methods.emplace_back(internalName, descriptor, node);
            }
        }
        else {
            std::cerr << "[Critical Error] currentClass is NULL while analyzing " << internalName << "\n";
        }
        std::cout << "[JvmGen] Method '" << internalName << "' processed. Locals count: " 
                << method.locals.size() << "\n";
    }

    // 4. Обработка Let-блоков
    else if (node->type == DECL_BLOCK && node->name == "LET_BLOCK") {
        // В DeclNode список let-переменных лежит в letBlock (ASTNode*, кастим)
        if (node->letBlock) {
            auto* list = dynamic_cast<DeclListNode*>(node->letBlock);
            if (list) {
                for (auto* varDecl : list->decls) {
                    analyzeDecl(varDecl);
                }
            }
        }
    }

    // Обработка var <- expr
    else if (node->type == NodeType::DECL_MONADIC_BIND) {
        if (node->expr) {
            // 1. Анализируем правую часть (там будет вызов функции с определённым типом)
            analyzeExpr(node->expr);

            // 2. Получаем тип переменной
            SemanticType* type = node->expr->inferredType;

            // 3. Выделяем индекс
            int index = nextLocalIndex++;
            node->localVarIndex = index;
            node->inferredType = type;

            // 4. Регистрируем в таблице символов
            symbolTable[node->name] = LocalVariable(type, index);

            std::cout << "[Semantic] Bind var '" << node->name 
                      << "' <- ... index=" << index 
                      << " type=" << type->getDescriptor() << "\n";
        }
    }

    else if (node->type == DECL_ACTION) {
        // У DeclNode есть поле expr (само выражение вызова функции)
        if (node->expr) {
            analyzeExpr(node->expr);
            // Тип действия совпадает с типом выражения
            if (dynamic_cast<ASTNode*>(node)) node->inferredType = node->expr->inferredType;
        }
    }

    else if (node->type == DECL_DATA) {
        std::cout << "[DEBUG] Inside DATA_DECL for: " << node->name << std::endl;
        // Создаем тип на основе имени узла
        SemanticType* enumType = SemanticType::Enum(node->name);
        
        // enumData - это дочерний список конструкторов
        if (node->enumData) {
            auto* constrList = static_cast<DeclListNode*>(node->enumData);
            int currentVal = 0;

            for (auto* constr : constrList->decls) {
                // Если у конструктора есть явное значение
                if (constr->expr) {
                    currentVal = std::stoi(static_cast<ExprNode*>(constr->expr)->value);
                }

                // Добавляем в таблицу
                symbolTable[constr->name] = LocalVariable::EnumConst(enumType, currentVal);
                
                // Сохраняем индекс в CP для генератора
                constr->constPoolIndex = constPool.addInteger(currentVal);
                std::cout << "[Semantic] Registered Enum: " << constr->name << " = " << currentVal << std::endl;
                currentVal++;
            }
        }
        return;
    }
}

void SemanticAnalyzer::analyzeExpr(ExprNode* node, SemanticType* expectedType) {
    if (!node) return;

    switch (node->type) {
    case EXPR_LITERAL: {
        if (node->value.empty()) {
            node->inferredType = SemanticType::String();
            node->constPoolIndex = constPool.addStringLiteral("");
            break;
        }

        // 1. ПЕРВООЧЕРЕДНАЯ ПРОВЕРКА: Если есть кавычки - это точно STRING
        if (node->value.front() == '"' && node->value.back() == '"') {
            node->inferredType = SemanticType::String();
            
            // Убираем кавычки для Constant Pool
            std::string cleanValue = node->value.substr(1, node->value.size() - 2);
            node->constPoolIndex = constPool.addStringLiteral(cleanValue);
            break;
        }

        // 2. ПРОВЕРКА НА БУЛ
        if (node->value == "True" || node->value == "False") {
            node->inferredType = SemanticType::Bool();
            // В байткоде Bool — это Int (0 или 1)
            int val = (node->value == "True" ? 1 : 0);
            node->constPoolIndex = constPool.addInteger(val); 
            break;
        }

        // 3. ПРОВЕРКА НА ЧИСЛО: Теперь проверяем, что ВСЯ строка числовая
        // (начинается с цифры или минуса, и не содержит лишнего)
        auto isFullNumeric = [](const std::string& s) {
            if (s.empty()) return false;
            size_t start = (s[0] == '-' && s.size() > 1) ? 1 : 0;
            bool hasDot = false;
            for (size_t i = start; i < s.size(); ++i) {
                if (s[i] == '.') {
                    if (hasDot) return false; // Две точки - это не число
                    hasDot = true;
                } else if (!isdigit(s[i])) {
                    return false; // Любой другой символ (буква, пробел) делает это строкой
                }
            }
            return true;
        };

        if (isFullNumeric(node->value)) {
            if (node->value.find('.') != std::string::npos) {
                node->inferredType = SemanticType::Float();
                node->constPoolIndex = constPool.addFloat(std::stof(node->value));
            } else {
                node->inferredType = SemanticType::Int();
                node->constPoolIndex = constPool.addInteger(std::stoi(node->value));
            }
        } 
        else {
            // 3. Если не в кавычках и не число (id или чертовщина от юзера)
            node->inferredType = SemanticType::String();
            node->constPoolIndex = constPool.addStringLiteral(node->value);
        }
        break;
    }

    case EXPR_PATTERN_VAR:
    case EXPR_VAR: {
        // 1. Проверка локальных переменных и констант Enum
        if (symbolTable.count(node->name)) {
            auto& info = symbolTable[node->name];
            node->inferredType = info.type;

            if (info.isEnum) {
                node->inferredType = info.type;
                node->value = std::to_string(info.enumValue);
                node->isEnumConstant = true;
                std::cout << "[Semantic] Resolved enum constant '" << node->name 
                        << "' with value " << info.enumValue << "\n";
                return;
            } else { // Обычная локальная переменная (аргумент функции или let binding)
                node->localVarIndex = info.index;
            }
        } 
        // 2. Проверка глобальных функций
        else if (functionSignatures.count(node->name)) {
            FunctionSignature& sig = functionSignatures[node->name];
            node->inferredType = SemanticType::Function(sig.paramTypes, sig.returnType);
            node->isFunctionRef = true;
        }
        // 3. Встроенные функции
        else if (builtinSignatures.count(node->name)) {
            FunctionSignature& sig = builtinSignatures[node->name];
            node->inferredType = sig.returnType;
            node->isBuiltinFunciton = true;

            std::string descriptor = "()" + sig.returnType->getDescriptor();
            node->constPoolIndex = constPool.addMethodRef("HaskellRuntime", node->name, descriptor);
        }
        break;
    }

    case EXPR_BINARY: {
        analyzeExpr(node->left);
        analyzeExpr(node->right);

        SemanticType* lType = node->left->inferredType;
        SemanticType* rType = node->right->inferredType;

        if (!lType || !rType) return;

        // --- Логические операторы (&&, ||) ---
        if (node->op == "&&" || node->op == "||") {
            if (lType->base != BaseType::BOOL || rType->base != BaseType::BOOL) {
                std::cerr << "[Semantic Error]: Logical ops require Bool, got " 
                        << lType->typeName << " and " << rType->typeName << "\n";
            }
            node->inferredType = SemanticType::Bool();
        }

        // --- Оператор CONS (:) ---
        else if (node->op == ":") {
            // Ожидаем: elem : [elem]
            if (rType->kind == TypeKind::LIST) {
                // Тут можно добавить строгую проверку lType == rType->subType
            } else {
                std::cerr << "[Semantic] Error: Right side of ':' must be a list\n";
            }
            // Результат - это тип правого операнда (список)
            node->inferredType = rType;
            
            // Добавляем MethodRef в пул констант заранее
            std::string runtimeMethod = "cons";
            std::string descriptor;
            
            if (lType->base == BaseType::INT) {
                descriptor = "(I[I)[I";
            } else if (lType->base == BaseType::FLOAT) {
                descriptor = "(F[F)[F";
            } else {
                descriptor = "(Ljava/lang/String;[Ljava/lang/String;)[Ljava/lang/String;";
            }
            node->constPoolIndex = constPool.addMethodRef("HaskellRuntime", runtimeMethod, descriptor);
        }

        // --- Сравнения (расширенный список) ---
        else if (node->op == "<=" || node->op == "==" || node->op == ">" || 
                node->op == "<"  || node->op == "!=" || node->op == ">=") {
            
            node->inferredType = SemanticType::Bool();

            // Авто-каст для примитивов (Int -> Float)
            if (lType->base == BaseType::FLOAT || rType->base == BaseType::FLOAT) {
                if (lType->base == BaseType::INT) {
                    node->left = createCastNode(node->left, SemanticType::Float());
                }
                if (rType->base == BaseType::INT) {
                    node->right = createCastNode(node->right, SemanticType::Float());
                }
            }
        }

        // --- Доступ к элементу массива ---
        else if (node->op == "!!") {
            // Left: [T], Right: Int, Result: T
            if (node->left->inferredType->kind == TypeKind::LIST) {
                node->inferredType = node->left->inferredType->subType;
            } else {
                node->inferredType = SemanticType::Unknown(); 
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
        // Тип определяется динамически
        node->inferredType = SemanticType::List(SemanticType::Unknown());
        break;

    case EXPR_LIST:
    case EXPR_ARRAY: {
        SemanticType* elemType = SemanticType::Unknown();

        if (!node->block.empty()) {
            analyzeExpr(node->block[0]);
            elemType = node->block[0]->inferredType;

            for (size_t i = 1; i < node->block.size(); ++i) {
                analyzeExpr(node->block[i], elemType); // Передаем тип первого элемента остальным
            }
        } else {
            if (expectedType && expectedType->kind == TypeKind::LIST) {
                // Если массив пустой, берем тип элементов из ожидаемого типа [T]
                elemType = expectedType->subType;
            } else {
                // Если контекста нет, по умолчанию считаем Int
                elemType = SemanticType::Int();
            }
        }

        node->inferredType = SemanticType::List(elemType);

        if (node->inferredType->subType &&
            node->inferredType->subType->base == BaseType::STRING) {
            node->constPoolIndex = constPool.addClass("java/lang/String");
        }
        break;
    }

    case EXPR_IF: {
        analyzeExpr(node->cond);
        if (node->cond->inferredType->base != BaseType::BOOL) {
            std::cerr << "Error: IF condition must be Boolean" << std::endl;
        }

        analyzeExpr(node->expr_true);
        analyzeExpr(node->expr_false);

        // Проверка совпадения типов веток
        if (node->expr_true->inferredType->base != node->expr_false->inferredType->base) {
            // Можно добавить автокаст к Float, если одна ветка Int, а другая Float
            std::cerr << "Error: IF branches type mismatch" << std::endl;
        }

        node->inferredType = node->expr_true->inferredType;
        break;
    }

    case EXPR_FUNC_CALL: {
        std::vector<ExprNode*> allArgs;
        ExprNode* baseFuncNode = nullptr;
        flattenCall(node, allArgs, &baseFuncNode);
        if (!baseFuncNode) break;

        node->flattenedArgs = allArgs;

        // 1. Анализируем саму базу функции (чтобы узнать её имя)
        analyzeExpr(baseFuncNode);
        std::string name = baseFuncNode->name;
        if (name.find(' ') != std::string::npos) name = name.substr(0, name.find(' '));

        for (auto* arg : allArgs) analyzeExpr(arg); 

        // 2. Ищем сигнатуру ДО анализа аргументов
        FunctionSignature sig;
        bool foundSig = false;
        bool isBuiltin = false;

        // Специальная логика для head/tail (динамическая сигнатура)
        if  ((name == "head" 
            || name == "tail" 
            || name == "last" 
            || name == "isNull") 
            && !allArgs.empty()) {
            SemanticType* argType = allArgs[0]->inferredType;
            
            // Работаем только если аргумент - список
            if (argType && argType->kind == TypeKind::LIST) {
                sig.paramTypes = { argType };
                
                if (name == "head" || name == "last")
                    sig.returnType = argType->subType;
                else if (name == "tail") 
                    sig.returnType = argType;
                else if (name == "isNull") 
                    sig.returnType = SemanticType::Bool(); // Динамически [T] -> Bool
                    
                foundSig = true;
                isBuiltin = true;
                baseFuncNode->inferredType = SemanticType::Function(sig.paramTypes, sig.returnType);
                baseFuncNode->isBuiltinFunciton = true;
            }
        }

        // Сигнатура для полиморфного print
        else if (name == "print" && !allArgs.empty()) {
            SemanticType* argType = allArgs[0]->inferredType;
            sig.paramTypes = { argType };
            sig.returnType = SemanticType::IO();
            
            foundSig = true;
            isBuiltin = true;

            baseFuncNode->inferredType = SemanticType::Function(sig.paramTypes, sig.returnType);
        }

        // Общая логика (из словарей)
        else if (builtinSignatures.count(name)) {
            sig = builtinSignatures[name];
            foundSig = true;
            isBuiltin = true;
        } else if (functionSignatures.count(name)) {
            sig = functionSignatures[name];
            foundSig = true;
            isBuiltin = false;
        }

        // 2. Если сигнатура определена (динамически или статически)
        if (foundSig) {
            // 3. Анализируем аргументы, зная их ожидаемые типы
            for (size_t i = 0; i < allArgs.size(); ++i) {
                SemanticType* expectedArgType = nullptr;
                if (i < sig.paramTypes.size()) {
                    expectedArgType = sig.paramTypes[i];
                } 
                analyzeExpr(allArgs[i], expectedArgType); // Передаем ожидаемый тип
            }

            // Типизация цепочки (чтобы не было ?)
            std::vector<ExprNode*> chain;
            ExprNode* curr = node;
            while (curr && curr->type == EXPR_FUNC_CALL) {
                chain.push_back(curr);
                curr = curr->function;
            }

            SemanticType* currentType = baseFuncNode->inferredType;
            for (int i = (int)chain.size() - 1; i >= 0; --i) {
                if (currentType->kind == TypeKind::FUNCTION && !currentType->paramTypes.empty()) {
                    if (currentType->paramTypes.size() > 1) {
                        std::vector<SemanticType*> rem(currentType->paramTypes.begin() + 1, currentType->paramTypes.end());
                        currentType = SemanticType::Function(rem, currentType->returnType);
                    } else {
                        currentType = currentType->returnType;
                    }
                }
                chain[i]->inferredType = currentType;
            }

            baseFuncNode->inferredType = SemanticType::Function(sig.paramTypes, sig.returnType);
            baseFuncNode->isBuiltinFunciton = isBuiltin; // Чтобы генератор знал, какой класс писать
        } else {
            for (auto* arg : allArgs) analyzeExpr(arg);
        }
        break;
    }

    case EXPR_DO_BLOCK: {
        if (node->decls) {
            for (auto* d : node->decls->decls) {
                analyzeDecl(d);
            }
        }
        node->inferredType = SemanticType::IO(); 
        break;
    }

    case EXPR_PATTERN_CONSTRUCTOR: {
        std::cout << "[DEBUG] Inside analyzeExpr::CONSTRUCTOR for: " << node->name << std::endl;
        // 1. Ищем конструктор в таблице
        if (symbolTable.count(node->name)) {
            auto& info = symbolTable[node->name];
            
            if (info.isEnum) {
                // 2. Устанавливаем тип
                node->inferredType = info.type;
                
                // 3. Перезаписываем value реальным типом
                node->value = std::to_string(info.enumValue); 
                
                std::cout << "[DEBUG-SEM] Patched " << node->name 
                        << " with int value: " << node->value << std::endl;
            }
        } else {
            std::cerr << "[Error] Unknown constructor: " << node->name << std::endl;
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

void SemanticAnalyzer::analyzePattern(ExprNode* pattern, SemanticType* expectedType, int& nextSlot, int argIdx) {
    if (!pattern) return;
    
    // Устанавливаем ожидаемый тип (Color)
    pattern->inferredType = expectedType; 

    // 1. Обычные переменные (x, y, a)
    if (pattern->type == EXPR_PATTERN_VAR) {
        int finalIdx = (argIdx != -1) ? argIdx : nextSlot++;
        pattern->localVarIndex = finalIdx;
        symbolTable[pattern->name] = LocalVariable(expectedType, finalIdx);
    }
    // 2. Списки (x:xs)
    else if (pattern->type == EXPR_PATTERN_CONS) {
        analyzePattern(pattern->left, expectedType->subType, nextSlot, -1);
        analyzePattern(pattern->right, expectedType, nextSlot, -1);
    }
    // 3. Конструкторы (перечисления)
    else if (pattern->type == EXPR_PATTERN_CONSTRUCTOR) {
        // --- ОТЛАДКА ---
        std::cout << "[DEBUG-SEM] Looking for: '" << pattern->name << "'" << std::endl;
        std::cout << "[DEBUG-SEM] Current table keys: ";
        for(auto const& pair : symbolTable) {
            std::cout << "'" << pair.first << "' "; // pair.first это key
        }
        std::cout << std::endl;
        // ---------------

        if (symbolTable.count(pattern->name)) {
            auto& info = symbolTable[pattern->name];
            if (info.isEnum) {
                // Уточняем тип (на случай если expectedType был общим)
                pattern->inferredType = info.type;
                
                // Записываем строковое представление числа для генератора
                pattern->value = std::to_string(info.enumValue);

                std::cout << "[Semantic-Pattern] Patched enum pattern '" << pattern->name 
                          << "' -> value " << pattern->value << std::endl;
            }
        } else {
            std::cerr << "[Semantic Error] Unknown pattern constructor: " << pattern->name << std::endl;
        }
    }
    // 4. (На будущее) Tuple
    else if (pattern->type == EXPR_PATTERN_TUPLE) {
        // Логика для ()
    }
}

void SemanticAnalyzer::collectTypes(ASTNode* node, std::vector<SemanticType*>& types) {
    if (!node) return;
    ExprNode* expr = dynamic_cast<ExprNode*>(node);
    if (!expr) return;

    if (expr->type == EXPR_TYPE_FUNCTION) {
        collectTypes(expr->left, types);
        collectTypes(expr->right, types);
    }
    else if (expr->type == EXPR_TYPE_PRIMITIVE) {
        if (expr->value == "Int") types.push_back(SemanticType::Int());
        else if (expr->value == "Float") types.push_back(SemanticType::Float());
        else if (expr->value == "Bool") types.push_back(SemanticType::Bool());
        else if (expr->value == "()" || expr->value == "Void") types.push_back(SemanticType::Void());
        else types.push_back(SemanticType::Unknown());
    }
    // Обработка конструкторов (IO, Maybe и т.д.)
    else if (expr->type == EXPR_TYPE_CONSTRUCTOR) {
        if (expr->name == "IO") {
            types.push_back(SemanticType::IO());
        } else {
            SemanticType* constr = new SemanticType(TypeKind::CONSTRUCTOR);
            constr->typeName = expr->name;
            types.push_back(constr);
        }
    }
    else if (expr->type == EXPR_TYPE_LIST) {
        std::vector<SemanticType*> inner;
        collectTypes(expr->right ? expr->right : expr->left, inner); 
        if (!inner.empty()) {
            types.push_back(SemanticType::List(inner.back()));
        } else {
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

void SemanticAnalyzer::initBuiltins() {
    builtinSignatures["isNull"] = { {}, SemanticType::Bool() };
    builtinSignatures["head"] = { {}, SemanticType::Unknown() };
    builtinSignatures["tail"] = { {}, SemanticType::Unknown() };
    builtinSignatures["last"] = { {}, SemanticType::Unknown() };


    // MAP

    SemanticType* anyA = SemanticType::Unknown();
    SemanticType* anyB = SemanticType::Unknown();
    SemanticType* mapFuncArg = SemanticType::Function({anyA}, anyB);
    builtinSignatures["map"] = { { mapFuncArg, SemanticType::List(anyA) }, SemanticType::List(anyB) };


    // FOLD

    SemanticType* anyAcc = SemanticType::Unknown();
    SemanticType* anyEl  = SemanticType::Unknown();

    // Первый аргумент: (acc -> el -> acc)
    SemanticType* foldFuncArg = SemanticType::Function({anyAcc, anyEl}, anyAcc);
    
    FunctionSignature foldSig;
    // Аргументы: функция, начальное значение (acc), список ([el])
    foldSig.paramTypes = { foldFuncArg, anyAcc, SemanticType::List(anyEl) };
    foldSig.returnType = anyAcc; // Результат — того же типа, что аккумулятор
    
    builtinSignatures["fold"] = foldSig;


    // IO функции
    builtinSignatures["putStrLn"] = { {SemanticType::String()}, SemanticType::IO() };
    builtinSignatures["print"] =    { {SemanticType::Unknown()}, SemanticType::IO() };

    builtinSignatures["readInt"]      = { {}, SemanticType::Int() };
    builtinSignatures["readFloat"]    = { {}, SemanticType::Float() };
    builtinSignatures["readString"]   = { {}, SemanticType::String() };
    builtinSignatures["readArrayInt"] = { {}, SemanticType::List(SemanticType::Int()) };
    builtinSignatures["readArrayFloat"] = { {}, SemanticType::List(SemanticType::Float()) };
    builtinSignatures["readArrayString"] = { {}, SemanticType::List(SemanticType::String()) };
}

void SemanticAnalyzer::flattenCall(ExprNode* node, std::vector<ExprNode*>& args, ExprNode** finalFunc) {
    if (node->type == EXPR_FUNC_CALL && node->function) {
        // Рекурсивно идем влево (вглубь вызова)
        flattenCall(dynamic_cast<ExprNode*>(node->function), args, finalFunc);
        // Собираем аргументы по пути назад
        for (auto* arg : node->arguments) {
            args.push_back(dynamic_cast<ExprNode*>(arg));
        }
    } else {
        // Мы дошли до самого корня — узла EXPR_VAR_REF (например, 'fold')
        *finalFunc = node;
    }
}


// PROCESSING

void SemanticAnalyzer::printDebugInfo() {
    std::cout << "\n============================================\n";
    std::cout << "           JVM CONSTANT POOL                \n";
    std::cout << "============================================\n";
    
    const auto& entries = constPool.getEntries();
    for (size_t i = 1; i < entries.size(); ++i) { // Индексы с 1
        const auto& e = entries[i];
        std::cout << "#" << i << " = ";
        
        switch (e.tag) {
            case CONSTANT_Utf8: 
                std::cout << "Utf8          \"" << e.stringValue << "\""; 
                break;
            case CONSTANT_Integer: 
                std::cout << "Integer       " << e.intValue; 
                break;
            case CONSTANT_Float: 
                std::cout << "Float         " << e.floatValue; 
                break;
            case CONSTANT_Class: 
                std::cout << "Class         #" << e.refIndex1; 
                break;
            case CONSTANT_String: 
                std::cout << "String        #" << e.refIndex1; 
                break;
            case CONSTANT_NameAndType: 
                std::cout << "NameAndType   #" << e.refIndex1 << ":#" << e.refIndex2; 
                break;
            case CONSTANT_Methodref: 
                std::cout << "MethodRef     #" << e.refIndex1 << ".#" << e.refIndex2; 
                break;
            default: 
                std::cout << "Unknown Tag (" << (int)e.tag << ")";
        }
        std::cout << "\n";
    }

    std::cout << "\n============================================\n";
    std::cout << "           CLASS METHODS                    \n";
    std::cout << "============================================\n";

    if (currentClass) {
        std::cout << "Class: " << currentClass->className << "\n";
        std::cout << "--------------------------------------------\n";
        for (const auto& m : currentClass->methods) {
            std::cout << "Method: " << m.name << "\n";
            std::cout << "  Descriptor: " << m.descriptor << "\n";
            std::cout << "  CP Indices: Name=#" << m.nameIdx << ", Desc=#" << m.descIdx << "\n";
            std::cout << "  Locals (" << m.locals.size() << "):\n";
            for (const auto& l : m.locals) {
                // Для красивого вывода нужно найти имя переменной. 
                // В SymbolTable мы хранили имя в ключе map, а LocalVariable хранит только индекс/тип.
                // В реальном коде можно добавить std::string name в LocalVariable для отладки.
                std::cout << "    Slot " << l.index << ": Type=" 
                          << (l.type ? l.type->toString() : "?") << "\n";
            }
            std::cout << "--------------------------------------------\n";
        }
    }
}