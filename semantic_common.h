#pragma once
#include <vector>
#include "semantic_type.h"

struct LocalVariable {
    SemanticType* type;
    int index;
    bool isEnum = false;
    int enumValue = 0;

    LocalVariable() : type(nullptr), index(-1) {}
    LocalVariable(SemanticType* t, int i) : type(t), index(i), isEnum(false) {}
    
    static LocalVariable EnumConst(SemanticType* t, int val) {
        LocalVariable lv;
        lv.type = t;
        lv.isEnum = true;
        lv.enumValue = val;
        return lv;
    }
};

struct FunctionSignature {
    std::vector<SemanticType*> paramTypes;
    SemanticType* returnType;
    std::string getDescriptor() const {
    std::string res = "(";
        for (auto* t : paramTypes) {
            if (t) {
                std::string d = t->getDescriptor();
                if (d != "V") res += d;
            }
        }
        res += ")";
        if (returnType) res += returnType->getDescriptor();
        else res += "V";
        return res;
    }
};