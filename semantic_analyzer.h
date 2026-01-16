#pragma once
#include <map>
#include <string>
#include "ast.h"
#include "jvm_class.h"
#include "constant_pool.h"
#include "semantic_common.h"

class SemanticAnalyzer {
private:
    JvmClass* currentClass = nullptr;
    ConstantPool& constPool;
    
    std::map<std::string, LocalVariable> symbolTable;
    std::map<std::string, FunctionSignature> functionSignatures;
    std::map<std::string, FunctionSignature> builtinSignatures;

    int nextLocalIndex = 1;

public:
    SemanticAnalyzer(ConstantPool& cp) : constPool(cp) { initBuiltins(); }
    void analyze(ProgramNode* root);
    void printDebugInfo();

private:
    void initBuiltins();

    void analyzeProgram(ProgramNode* root);

    void analyzeDeclList(DeclListNode* list);
    void analyzeDecl(DeclNode* node);
    void analyzeExpr(ExprNode* node);
    void analyzePattern(ExprNode* pattern, SemanticType* expectedType, int& sourceLocalIndex);

    void collectTypes(ASTNode* node, std::vector<SemanticType*>& types);
    void flattenCall(ExprNode* node, std::vector<ExprNode*>& args, ExprNode** finalFunc);
    
    ExprNode* createCastNode(ExprNode* target, SemanticType* toType);
};