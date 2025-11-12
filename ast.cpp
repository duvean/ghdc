#include <iostream>
#include "ast.h"

std::string getPatternName(ExprNode* patternNode) {
    if (patternNode) {
        return patternNode->name;
    }
    return "NULL_PATTERN";
}

std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NODE_PROGRAM:     return "PROGRAM";
        case DECL_VAR:         return "VAR_DECL";
        case DECL_FUNC:        return "FUNC_DECL";
        case DECL_LIST:        return "DECL_LIST";
        case DECL_DATA:        return "DECL_DATA";
        case DECL_TYPE:        return "DECL_TYPE";
        default:               return "UNKNOWN";
    }
}

std::string getDeclName(DeclNode* node) {
    if (!node) {
        return "NULL";
    }

    // 1. Имя, явно присвоенное узлу (для func_signature)
    if (!node->name.empty()) {
        return node->name; 
    }

    // 2. Имя извлечено из связанного выражения/паттерна (для параметров/связываний)
    // Это должно сработать для {VAR_DECL} a и b.
    if (node->expr) {
        // Требуется, чтобы ExprNode::getName() возвращал "a", "b", "x" и т.д.
        std::string exprName = node->expr->name; 
        if (!exprName.empty()) {
            return exprName;
        }
        return "name_empty_somehow";
    }

    // 3. Узел-контейнер/блок без имени
    // Для лучшей читаемости используйте более информативный тип, 
    // например, NODE_LET_BLOCK вместо DECL_VAR, если он используется для блока where/let.
    return "{" + nodeTypeToString(node->type) + "}";
}

std::string DeclListNode::listToString() const {
    std::string result = "";
    for (size_t i = 0; i < decls.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += getDeclName(decls[i]);
    }
    return result;
}

ProgramNode* ProgramNode::create(DeclListNode* declList) {
    ProgramNode* root = new ProgramNode();
    if (declList) {
        root->decls = std::move(declList->decls); 
    }
    std::cout << "createProgram\n";
    return root;
}

DeclNode* DeclNode::createVarDecl(const std::string& name, ExprNode* expr) {
    DeclNode* node = new DeclNode(NodeType::DECL_VAR); 
    
    node->name = name;
    node->expr = expr;
    
    std::cout << "createVarDecl(" << name << ") -> Expression: " 
              << (expr ? "present" : "NULL") << "\n";
    return node;
}

DeclNode* DeclNode::createFuncDef(const std::string& name, 
                                  ASTNode* params, 
                                  ExprNode* body, 
                                  ASTNode* whereBlock) 
{
    DeclNode* node = new DeclNode(NodeType::DECL_FUNC);
    
    node->name = name;
    node->expr = body; // Тело функции (правая часть '=')
    node->paramsList = params; 
    node->whereBlock = whereBlock; // Блок локальных объявлений
    
    // --- Отладочный вывод ---
    
    // Размер списка параметров для трассировки:
    size_t paramCount = 0;
    // Явное приведение, так как params - это ASTNode*, который фактически DeclListNode*
    DeclListNode* paramList = static_cast<DeclListNode*>(params);
    if (paramList) {
        paramCount = paramList->decls.size();
    }
    
    std::cout << "createFuncDef(" << name << ") -> "
              << "Params: " << paramCount
              << ", Body: " << (body ? "present" : "NULL") 
              << ", Where: " << (whereBlock ? "present" : "NULL") << "\n";
              
    return node;
}

DeclNode* DeclNode::createFuncSignature(const std::string& name, ASTNode* typeExpr) {
    DeclNode* node = new DeclNode(NodeType::DECL_TYPE); 
    
    node->name = name;
    node->typeExpr = typeExpr;
    
    std::cout << "createFuncSignature(" << name << ") -> Type: " 
              << (typeExpr ? "present" : "NULL") << "\n";
    return node;
}

DeclNode* DeclNode::createDataDecl(const std::string& name, ASTNode* constructors) {
    DeclNode* node = new DeclNode(NodeType::DECL_DATA);
    
    node->name = name;
    node->whereBlock = constructors; // Используем whereBlock для сохранения списка конструкторов
    
    std::cout << "createDataDecl(" << name << ") -> Constructors: " 
              << (constructors ? "present" : "NULL") << "\n";
    return node;
}

DeclNode* DeclNode::createTypeDecl(const std::string& name, ASTNode* typeExpr) {
    DeclNode* node = new DeclNode(NodeType::DECL_TYPE);
    
    node->name = name;
    node->typeExpr = typeExpr; // Выражение, описывающее тип (правая часть '=')
    
    std::cout << "createTypeDecl(" << name << ") -> Type: " 
              << (typeExpr ? "present" : "NULL") << "\n";
    return node;
}

DeclNode* DeclNode::createLetBlock(DeclListNode* declList) {
    // Узел, представляющий блок локальных объявлений
    DeclNode* node = new DeclNode(NodeType::DECL_BLOCK); 
    
    node->whereBlock = declList; // Сохраняем список локальных объявлений
    node->name = "LET_BLOCK"; // Имя для отладки

    size_t size = declList ? declList->decls.size() : 0;
    std::cout << "createLetBlock: created node containing list of size " 
              << size << "\n";
    return node;
}

DeclNode* DeclNode::createParameter(ExprNode* patternNode) {
    // Параметр — это объявление, где имя может быть извлечено из паттерна.
    DeclNode* node = new DeclNode(NodeType::DECL_VAR);
    
    // Храним паттерн в поле expr.
    node->expr = patternNode;
    
    // NOTE: Если паттерн является простым ID, мы можем извлечь имя для node->name:
    // if (patternNode && patternNode->type == NodeType::EXPR_PATTERN_VAR) {
    //     node->name = patternNode->name;
    // }
    
    std::cout << "createParameter: created DeclNode for parameter.\n";
    return node;
}





/* --- DeclList --- */
DeclListNode* DeclListNode::create(DeclNode* first) {
    DeclListNode* list = new DeclListNode();
    std::string declName = getDeclName(first); 

    if (first) {
        list->decls.push_back(first);
    }
    
    std::cout << "createDeclList. Contains: [" << declName << "]. Size: 1\n";
    return list;
}

DeclListNode* DeclListNode::addDecl(DeclListNode* list, DeclNode* new_decl) {
    if (list && new_decl) {
        list->decls.push_back(new_decl);
    }
    
    // Получаем полное содержимое списка ПОСЛЕ добавления
    std::string content = list ? list->listToString() : "NULL";
    size_t currentSize = list ? list->decls.size() : 0;
    
    std::cout << "addDeclToList. Added: " << getDeclName(new_decl) 
              << ". Contains: [" << content << "]. Size: " << currentSize << "\n";
    
    return list;
}

DeclListNode* DeclListNode::createParamList(ExprNode* patternNode) {
    DeclNode* param_decl = DeclNode::createParameter(patternNode); 
    return DeclListNode::create(param_decl);
}

DeclListNode* DeclListNode::addParamToList(DeclListNode* list, ExprNode* patternNode) {
    DeclNode* param_decl = DeclNode::createParameter(patternNode); 
    return DeclListNode::addDecl(list, param_decl);
}

DeclListNode* DeclListNode::createConstructorList(char* name) {
    DeclNode* constructor_decl = new DeclNode(NodeType::DECL_CONSTRUCTOR);
    constructor_decl->name = name;

    return DeclListNode::create(constructor_decl);
}

DeclListNode* DeclListNode::addConstructorToList(DeclListNode* list, char* name) {
    DeclNode* new_constructor_decl = new DeclNode(NodeType::DECL_CONSTRUCTOR);
    new_constructor_decl->name = name;

    return DeclListNode::addDecl(list, new_constructor_decl);
}

/* --- Expr --- */

std::string getExprDescription(ExprNode* node) {
    if (!node) return "NULL";

    // 1. FuncCall (проверяем function != nullptr)
    if (node->function != nullptr) return "FuncCall(" + getExprDescription(node->function) + ")";

    // 2. VarRef
    if (node->type == EXPR_VAR) return "VarRef(" + node->name + ")";
    
    // 3. Literal
    if (node->type == EXPR_LITERAL) return "Literal(" + node->value + ")";
    
    // Для более сложных узлов (например, BINARY, IF)
    if (!node->op.empty()) return "Binary(" + node->op + ")"; 

    // Заглушка, если нет полей:
    return "Complex/Pattern"; 
}

// --- Новый метод для вывода содержимого списка ---
// Предполагаем, что list — это список, который содержит элементы в list->block
std::string listExprContent(ExprNode* list) {
    if (!list || list->block.empty()) return "";
    
    std::string content;
    for (size_t i = 0; i < list->block.size(); ++i) {
        if (i > 0) content += ", ";
        content += getExprDescription(list->block[i]);
    }
    return content;
}



ExprNode* ExprNode::createLiteral(const std::string& val) {
    std::cout << "createLiteral(" << val << ")\n";
    ExprNode* node = new ExprNode(NodeType::EXPR_LITERAL);
    node->value = val;
    return node;
}

ExprNode* ExprNode::createVarRef(const std::string& name) {
    std::cout << "createVarRef(" << name << ")\n";
    ExprNode* node = new ExprNode(NodeType::EXPR_VAR); // Используем EXPR_VAR для VarRef
    node->name = name;
    return node;
}

ExprNode* ExprNode::createBinaryExpr(const std::string& op, ExprNode* left, ExprNode* right) {
    ExprNode* node = new ExprNode(NodeType::EXPR_BINARY);
    node->op = op;
    node->left = left;
    node->right = right;
    
    std::cout << "createBinaryExpr(" << op << ") -> L: " 
              << getExprDescription(left) << ", R: " 
              << getExprDescription(right) << "\n";
    return node;
}

ExprNode* ExprNode::createIfExpr(ExprNode* cond, ExprNode* t, ExprNode* f) {
    ExprNode* node = new ExprNode(NodeType::EXPR_IF);
    node->cond = cond;
    node->expr_true = t;
    node->expr_false = f;
    
    std::cout << "createIfExpr -> Cond: " << getExprDescription(cond) << "\n";
    return node;
}

ExprNode* ExprNode::createArrayExpr(ASTNode* elements) {
    ExprNode* node = new ExprNode(NodeType::EXPR_ARRAY);
    // Предполагаем, что elements — это ExprNode* (список элементов)
    ExprNode* elementList = static_cast<ExprNode*>(elements); 

    // Если elementList — это узел списка, мы можем скопировать его block или сохранить ссылку
    if (elementList && elementList->type == NodeType::EXPR_LIST) {
        node->block = elementList->block; // Копируем или присваиваем элементы
        // В реальном коде: удаляем узел elementList, если он больше не нужен
    } else {
        // Или просто сохраняем сам список как левую ветвь для простоты
        node->left = elementList; 
    }
    
    std::cout << "createArrayExpr\n";
    return node;
}

ExprNode* ExprNode::createTupleExpr(ASTNode* elements) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TUPLE);
    node->left = static_cast<ExprNode*>(elements);
    
    std::cout << "createTupleExpr\n";
    return node;
}

ExprNode* ExprNode::createLambda(ExprNode* params, ExprNode* body) {
    ExprNode* node = new ExprNode(NodeType::EXPR_LAMBDA);
    
    // params (ExprNode*), который, вероятно, является EXPR_LIST или DeclNode* // со списком параметров (паттернов).
    node->left = params; 
    
    // body
    node->right = body; 
    
    std::cout << "createLambda -> Params: " 
              << (params ? getExprDescription(params) : "NULL") 
              << ", Body: " << (body ? getExprDescription(body) : "NULL") << "\n";
    return node;
}

ExprNode* ExprNode::createCaseExpr(ExprNode* cond, ASTNode* branches) {
    ExprNode* node = new ExprNode(NodeType::EXPR_CASE);
    node->cond = cond;
    // Предполагаем, что branches является ExprNode* или контейнером, 
    // который можно сохранить в одном из полей. Используем 'right' для списка ветвей.
    node->right = static_cast<ExprNode*>(branches); 
    
    std::cout << "createCaseExpr -> Cond: " << getExprDescription(cond) << "\n";
    return node;
}

ExprNode* ExprNode::createLetInExpr(ASTNode* decls, ExprNode* expr) {
    ExprNode* node = new ExprNode(NodeType::EXPR_LET_IN);
    // Предполагаем, что decls (список DeclNode*) можно сохранить в 'left'.
    node->left = static_cast<ExprNode*>(decls); 
    node->right = expr; // Выражение 'in'
    
    std::cout << "createLetInExpr -> Body: " << getExprDescription(expr) << "\n";
    return node;
}

ExprNode* ExprNode::createDoExpr(ASTNode* block) {
    ExprNode* node = new ExprNode(NodeType::EXPR_DO);
    
    // Блок DO-выражений (binds/exprs) лучше всего хранить в `block`.
    // Предполагаем, что входящий ASTNode* block — это ExprNode* типа EXPR_LIST.
    ExprNode* blockList = static_cast<ExprNode*>(block);
    
    if (blockList && blockList->type == NodeType::EXPR_LIST) {
        node->block = blockList->block; // Копируем список выражений/связываний
    } else {
        node->left = blockList; // Если это не EXPR_LIST, сохраняем как отдельный узел
    }
    
    std::cout << "createDoExpr -> Block: " 
              << (blockList ? "present" : "NULL") << "\n";
    return node;
}

ExprNode* ExprNode::createReturnExpr(ExprNode* expr) {
    ExprNode* node = new ExprNode(NodeType::EXPR_RETURN);
    node->left = expr; 
    std::cout << "createReturnExpr -> Expr: " 
              << (expr ? getExprDescription(expr) : "NULL") << "\n";
    return node;
}

ExprNode* ExprNode::createExprList(ExprNode* first) {
    ExprNode* list = new ExprNode(NodeType::EXPR_LIST);
    list->block.push_back(first);
    
    std::cout << "createExprList. Start: [" << getExprDescription(first) << "]\n";
    return list;
}

ExprNode* ExprNode::addExprToList(ExprNode* list, ExprNode* newExpr) {
    // В отличие от заглушки, мы теперь предполагаем, что list — это контейнер EXPR_LIST
    
    std::string added = getExprDescription(newExpr);
    std::string oldContent = (list && !list->block.empty()) 
                            ? getExprDescription(list->block[0]) : "Empty"; 

    if (list && list->type == NodeType::EXPR_LIST) {
        list->block.push_back(newExpr);
    }
    
    std::cout << "addExprToList. Added: " << added 
              << " to list starts with: [" << oldContent << "]\n";
    
    return list;
}

ExprNode* ExprNode::createFuncCall(ExprNode* funcExpr, ExprNode* argExpr) {
    ExprNode* res = new ExprNode(NodeType::EXPR_FUNC_CALL);
    res->function = funcExpr;
    res->argument = argExpr;
    
    std::cout << "createFuncCall. Func: " << getExprDescription(funcExpr) 
              << ", Arg: " << getExprDescription(argExpr) << "\n";
    return res;
}

ExprNode* ExprNode::createVarPattern(const std::string& identifier) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_VAR);
    node->name = identifier;
    std::cout << "createVarPattern:" << identifier << "\n";
    return node;
}

ExprNode* ExprNode::createLiteralPattern(const std::string& literalValue) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_LITERAL);
    node->value = literalValue;
    std::cout << "createLiteralPattern:" << literalValue << "\n";
    return node;
}

ExprNode* ExprNode::createConstructorPattern(const std::string& constructorName, ExprNode* args) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_CONSTRUCTOR);
    node->name = constructorName;
    node->argument = args; // Аргументы конструктора (если есть)
    std::cout << "createConstructorPattern:" << constructorName << "\n";
    return node;
}

ExprNode* ExprNode::createTuplePattern(ExprNode* patternList) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_TUPLE);
    node->left = patternList;
    std::cout << "createTuplePattern\n";
    return node;
}

ExprNode* ExprNode::createListPattern(ExprNode* patternList) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_LIST);
    node->left = patternList;
    std::cout << "createListPattern\n";
    return node;
}

ExprNode* ExprNode::createConsPattern(ExprNode* headPattern, ExprNode* tailPattern) {
    ExprNode* node = new ExprNode(NodeType::EXPR_PATTERN_CONS);
    node->left = headPattern;
    node->right = tailPattern;
    std::cout << "createConsPattern\n";
    return node;
}

ExprNode* ExprNode::addPatternToList(ExprNode* existingList, ExprNode* newPattern) {
    std::string added = getExprDescription(newPattern);
    std::string currentHead = (existingList && !existingList->block.empty()) 
                              ? getExprDescription(existingList->block[0]) : "Empty"; 
    
    if (existingList && existingList->type == NodeType::EXPR_LIST) {
        existingList->block.push_back(newPattern);
    }
    
    std::cout << "addPatternToList. New element: " << added 
              << ". After element: [" << currentHead << "]\n"; 
    
    return existingList; 
}

ExprNode* ExprNode::createPatternList(ExprNode* singlePattern) {
    ExprNode* list = new ExprNode(NodeType::EXPR_LIST); // Используем EXPR_LIST для паттерн-списков
    list->block.push_back(singlePattern);
    std::cout << "createPatternList. Start: [" << getExprDescription(singlePattern) << "]\n";
    return list; 
}





// Базовые типы (KW_INT, KW_FLOAT, ...)
ExprNode* ExprNode::createPrimitiveType(const std::string& name) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TYPE_PRIMITIVE);
    node->value = name;
    std::cout << "createPrimitiveType(" << name << ")\n";
    return node;
}

// Конструктор типа (ID_CAP, напр., Maybe, List)
ExprNode* ExprNode::createTypeConstructor(char* name) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TYPE_CONSTRUCTOR);
    node->name = name;
    std::cout << "createTypeConstructor(" << name << ")\n";
    return node;
}

// Переменная типа (ID, напр., a, b)
ExprNode* ExprNode::createTypeVar(char* name) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TYPE_VAR);
    node->name = name;
    std::cout << "createTypeVar(" << name << ")\n";
    return node;
}

// Функциональный тип (type_expr -> type_expr)
ExprNode* ExprNode::createFunctionType(ExprNode* argType, ExprNode* returnType) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TYPE_FUNCTION);
    node->left = argType;
    node->right = returnType;
    std::cout << "createFunctionType\n";
    return node;
}

// Аннотация типа (expr :: type_expr)
ExprNode* ExprNode::createTypeAnnotation(ExprNode* expr, ExprNode* typeExpr) {
    ExprNode* node = new ExprNode(NodeType::EXPR_TYPE_ANNOTATION);
    node->left = expr;
    node->right = typeExpr;
    std::cout << "createTypeAnnotation\n";
    return node;
}








// Создание одной ветви (pattern -> expr)
ExprNode* ExprNode::createCaseBranch(ExprNode* pattern, ExprNode* expr) {
    ExprNode* node = new ExprNode(NodeType::EXPR_CASE_BRANCH);
    node->left = pattern;
    node->right = expr;
    std::cout << "createCaseBranch\n";
    return node;
}

// Создание списка веток (Базовый случай)
ExprNode* ExprNode::createCaseBranchList(ExprNode* branch) {
    ExprNode* list = new ExprNode(NodeType::EXPR_CASE_BRANCH_LIST);
    
    // Ветка (branch) должна быть узлом EXPR_CASE_BRANCH
    list->block.push_back(branch); 
    std::cout << "createCaseBranchList. Start\n";
    return list;
}

// Добавление ветви в список
ExprNode* ExprNode::addCaseBranchToList(ExprNode* list, ExprNode* newBranch) {
    if (list && list->type == NodeType::EXPR_CASE_BRANCH_LIST) {
        list->block.push_back(newBranch);
    }
    std::cout << "addCaseBranchToList. Added branch. Total size: " 
              << (list ? list->block.size() : 0) << "\n";
    return list;
}
