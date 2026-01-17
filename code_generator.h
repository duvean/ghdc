#pragma once
#include <vector>
#include "ast.h"
#include "jvm_class.h"
#include "constant_pool.h"
#include "bytecode_emitter.h"

class CodeGenerator {
    ConstantPool& cp;
    BytecodeEmitter emit;

public:
    void generate(JvmMethod& method);

    CodeGenerator(ConstantPool& p, std::vector<uint8_t>& out) 
        : cp(p), emit(out) {}

private:
    void generateExpr(ExprNode* node);
    void generateDecl(DeclNode* node);
    
    uint8_t getStoreOpcode(SemanticType* type);
    uint8_t getLoadOpcode(SemanticType* type);
    uint8_t getReturnOpcode(const std::string& desc);
};