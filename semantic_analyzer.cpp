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

    // 1. Обработка переменных (let a = 1)
    if (node->type == DECL_VAR) {
        if (node->expr) {
            analyzeExpr(node->expr); // 1. Анализируем правую часть (1.5)
            
            // 2. Регистрируем переменную
            LocalVariable varInfo;
            varInfo.index = nextLocalIndex++;
            varInfo.type = node->expr->inferredType; // Тип берем из правой части
            
            symbolTable[node->name] = varInfo; // Добавляем в таблицу (a -> Float)
            
            // 3. Атрибутируем узел
            node->localVarIndex = varInfo.index;
            node->inferredType = SemanticType::Void;
            
            std::cout << "[Semantic] Declared var '" << node->name 
                      << "' index=" << varInfo.index 
                      << " type=" << typeToString(varInfo.type) << "\n";
        }
    }
    // 2. Обработка функций (main = ...)
    else if (node->type == DECL_FUNC) {
        std::cout << "[Semantic] Entering function: " << node->name << "\n";

        // A. Сначала обрабатываем where-блок, если он есть
        // Переменные из where должны попасть в таблицу ДО анализа тела функции
        if (node->whereBlock) {
            DeclListNode* whereList = dynamic_cast<DeclListNode*>(node->whereBlock);
            if (whereList) {
                analyzeDeclList(whereList);
            } else {
                // Если вдруг это не список, а одиночный DeclNode (зависит от парсера)
                DeclNode* whereDecl = dynamic_cast<DeclNode*>(node->whereBlock);
                if (whereDecl) analyzeDecl(whereDecl);
            }
        }

        // B. Анализируем тело функции (a + b)
        if (node->expr) {
            analyzeExpr(node->expr);
            // Тип функции выводится из типа её тела
            node->inferredType = node->expr->inferredType; 
            
            std::cout << "[Semantic] Function '" << node->name 
                      << "' return type: " << typeToString(node->inferredType) << "\n";
        }
    }
    // 3. Обработка Let-блоков (если они приходят как отдельный DeclNode)
    else if (node->type == DECL_BLOCK) {
         // Если letBlock хранит внутри DeclListNode
         DeclListNode* list = dynamic_cast<DeclListNode*>(node->letBlock);
         if (list) analyzeDeclList(list);
    }
}

void SemanticAnalyzer::analyzeExpr(ExprNode* node) {
    if (!node) return;

    switch (node->type) {
        case EXPR_LITERAL: {
            // Пытаемся определить тип литерала (примитивно по наличию точки)
            if (node->value.find('.') != std::string::npos) {
                node->inferredType = SemanticType::Float;
                float val = std::stof(node->value);
                node->constPoolIndex = constPool.addFloat(val);
            } else if (isdigit(node->value[0]) || node->value[0] == '-') { // Упрощенно
                node->inferredType = SemanticType::Int;
                int val = std::stoi(node->value);
                node->constPoolIndex = constPool.addInteger(val);
            } else {
                // Строка
                node->inferredType = SemanticType::String;
                // Убираем кавычки если они есть в парсере, или храним как есть
                node->constPoolIndex = constPool.addStringLiteral(node->value);
            }
            break;
        }

        case EXPR_VAR: {
            // Ищем в таблице символов
            if (symbolTable.count(node->name)) {
                LocalVariable info = symbolTable[node->name];
                node->localVarIndex = info.index;
                node->inferredType = info.type;
            } else {
                std::cerr << "[Semantic Error] Undefined variable: " << node->name << "\n";
                node->inferredType = SemanticType::Unknown;
            }
            break;
        }

        case EXPR_BINARY: {
            analyzeExpr(node->left);
            analyzeExpr(node->right);

            SemanticType lType = node->left->inferredType;
            SemanticType rType = node->right->inferredType;

            // Логика неявного преобразования (Int -> Float)
            if (lType == SemanticType::Float || rType == SemanticType::Float) {
                node->inferredType = SemanticType::Float;
                
                if (lType == SemanticType::Int) {
                    node->left = createCastNode(node->left, SemanticType::Float);
                    std::cout << "[Semantic] Auto-cast LEFT to Float\n";
                }
                if (rType == SemanticType::Int) {
                    node->right = createCastNode(node->right, SemanticType::Float);
                    std::cout << "[Semantic] Auto-cast RIGHT to Float\n";
                }
            } 
            else if (lType == SemanticType::Int && rType == SemanticType::Int) {
                node->inferredType = SemanticType::Int;
            }
            else {
                node->inferredType = SemanticType::Unknown; // Для строк и прочего
            }
            break;
        }

        case EXPR_CASTING:
            break; // Уже обработан

        default:
            // Для остальных рекурсивно
            if (node->left) analyzeExpr(node->left);
            if (node->right) analyzeExpr(node->right);
            break;
    }
}

std::string SemanticAnalyzer::makeMethodDescriptor(DeclNode* funcNode) {
    // Пока упрощенно: считаем, что аргументов нет (нужно доработать парсинг аргументов)
    // И возвращаем тип, который мы вывели (inferredType)
    std::string retDesc = getJvmDescriptor(funcNode->inferredType); 
    return "()" + retDesc;
}

// Создание узла кастинга. Оборачиваем старый узел в новый.
ExprNode* SemanticAnalyzer::createCastNode(ExprNode* target, SemanticType toType) {
    ExprNode* castNode = new ExprNode(EXPR_CASTING);
    castNode->left = target; // Оборачиваемый узел кладем в left
    castNode->inferredType = toType;
    castNode->isCastNode = true;
    return castNode;
}