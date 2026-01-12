#pragma once
#include <map>
#include <string>
#include <vector>
#include "ast.h"
#include "constant_pool.h"

struct LocalVariable {
    int index;
    SemanticType type;
};

class SemanticAnalyzer {
private:
    ConstantPool& constPool;
    
    // Таблица символов: Имя переменной -> Информаця
    std::map<std::string, LocalVariable> symbolTable;
    int nextLocalIndex = 1;

public:
    SemanticAnalyzer(ConstantPool& cp) : constPool(cp) {}
    void analyze(ProgramNode* root);

private:
    void analyzeDeclList(DeclListNode* list);
    void analyzeDecl(DeclNode* node);
    void analyzeExpr(ExprNode* node);

    std::string makeMethodDescriptor(DeclNode* funcNode);
    
    ExprNode* createCastNode(ExprNode* target, SemanticType toType);
};