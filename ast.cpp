#include <iostream>
#include "ast.h"

ProgramNode* ProgramNode::create(const std::vector<DeclNode*>& decls) {
    ProgramNode* node = new ProgramNode();
    node->decls = decls;
    std::cout << "createProgram\n";
    return node;
}

DeclNode* DeclNode::addDeclToList(DeclNode* list, DeclNode* newDecl) {
    std::cout << "addDeclToList\n";
    return list;
}

DeclNode* DeclNode::createDeclList(DeclNode* decl) {
    std::cout << "createDeclList\n";
    return decl;
}

DeclNode* DeclNode::createVarDecl(const std::string& name, ExprNode* expr) {
    std::cout << "createVarDecl(" << name << ")\n";
    return new DeclNode(name);
}

DeclNode* DeclNode::createFuncDef(const std::string& name, 
                                  ASTNode* params, 
                                  ExprNode* body, 
                                  ASTNode* whereBlock) 
{
    DeclNode* node = new DeclNode(NodeType::DECL_FUNC);
    node->name = name;
    node->expr = body; 
    std::cout << "createFuncDef(" << name << ")\n";
    return node;
}

DeclNode* DeclNode::createFuncSignature(const std::string& name, ASTNode* typeExpr) {
    std::cout << "createFuncSignature(" << name << ")\n";
    return new DeclNode(name);
}

DeclNode* DeclNode::createParamList(const std::string& name) {
    DeclNode* param_node = new DeclNode(NodeType::DECL_VAR);
    param_node->name = name;

    DeclNode* list = new DeclNode(NodeType::NODE_PROGRAM);
    list->block.push_back(param_node);
    std::cout << "createParamList(" << name << ")\n";
    return list;
}

DeclNode* DeclNode::addParamToList(DeclNode* list, const std::string& name) {
    if (list) {
        DeclNode* param_node = new DeclNode(NodeType::DECL_VAR);
        param_node->name = name;
        
        list->block.push_back(param_node);
    }
    std::cout << "addToParamList(" << name << ")\n";
    return list;
}

DeclNode* DeclNode::createDataDecl(const std::string& name, ASTNode* constructors) {
    std::cout << "createDataDecl(" << name << ")\n";
    return new DeclNode(name);
}

DeclNode* DeclNode::createTypeDecl(const std::string& name, ASTNode* typeExpr) {
    DeclNode* node = new DeclNode(NodeType::DECL_TYPE);
    node->name = name;
    node->typeExpr = typeExpr;
    std::cout << "createTypeDecl(" << name << ")\n";
    return node;
}

ExprNode* ExprNode::createLiteral(const std::string& val) {
    std::cout << "createLiteral(" << val << ")\n";
    return new ExprNode(val);
}

ExprNode* ExprNode::createVarRef(const std::string& name) {
    std::cout << "createVarRef(" << name << ")\n";
    return new ExprNode(name);
}

ExprNode* ExprNode::createBinaryExpr(const std::string& op, ExprNode* left, ExprNode* right) {
    std::cout << "createBinaryExpr(" << op << ")\n";
    return new ExprNode("binary");
}

ExprNode* ExprNode::createIfExpr(ExprNode* cond, ExprNode* t, ExprNode* f) {
    std::cout << "createIfExpr\n";
    return new ExprNode("if");
}

ExprNode* ExprNode::createArrayExpr(ASTNode* elements) {
    std::cout << "createArrayExpr\n";
    return new ExprNode("array");
}

ExprNode* ExprNode::createTupleExpr(ASTNode* elements) {
    std::cout << "createTupleExpr\n";
    return new ExprNode("tuple");
}

ExprNode* ExprNode::createLambda(ExprNode* params, ExprNode* body) {
    std::cout << "createLambda\n";
    return new ExprNode("lambda");
}

ExprNode* ExprNode::createCaseExpr(ExprNode* cond, ASTNode* branches) {
    std::cout << "createCaseExpr\n";
    return new ExprNode("case");
}

ExprNode* ExprNode::createLetInExpr(ASTNode* decls, ExprNode* expr) {
    std::cout << "createLetInExpr\n";
    return new ExprNode("let-in");
}

ExprNode* ExprNode::createDoExpr(ASTNode* block) {
    std::cout << "createDoExpr\n";
    return new ExprNode("do");
}

ExprNode* ExprNode::createReturnExpr(ExprNode* expr) {
    std::cout << "createReturnExpr\n";
    return new ExprNode("return");
}

ExprNode* ExprNode::createExprList(ExprNode* first) {
    std::cout << "createExprList\n";
    return first;
}

ExprNode* ExprNode::addExprToList(ExprNode* list, ExprNode* newExpr) {
    std::cout << "addExprToList\n";
    return list;
}
