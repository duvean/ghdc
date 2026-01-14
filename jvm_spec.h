#pragma once
#include <string>

enum class TypeKind { PRIMITIVE, LIST, UNKNOWN };
enum class BaseType { INT, FLOAT, BOOL, STRING, VOID };

enum JvmConstantTag {
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_Class = 7,
    CONSTANT_String = 8,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10,
    CONSTANT_NameAndType = 12
};

class SemanticType {
public:
    TypeKind kind;
    BaseType base;
    SemanticType* subType;

    SemanticType(BaseType b) : kind(TypeKind::PRIMITIVE), base(b), subType(nullptr) {}
    SemanticType(TypeKind k, SemanticType* sub) : kind(k), base(BaseType::VOID), subType(sub) {}
    SemanticType() : kind(TypeKind::UNKNOWN), base(BaseType::VOID), subType(nullptr) {}

    static SemanticType* Int()     { return new SemanticType(BaseType::INT); }
    static SemanticType* Float()   { return new SemanticType(BaseType::FLOAT); }
    static SemanticType* Bool()    { return new SemanticType(BaseType::BOOL); }
    static SemanticType* String()  { return new SemanticType(BaseType::STRING); }
    static SemanticType* Void()    { return new SemanticType(BaseType::VOID); }
    static SemanticType* Unknown() { return new SemanticType(); }
    static SemanticType* List(SemanticType* inner) {
        return new SemanticType(TypeKind::LIST, inner);
    }
    
    bool equals(SemanticType* other) {
        if (!other) return false;
        if (kind != other->kind) return false;
        if (kind == TypeKind::PRIMITIVE) return base == other->base;
        if (kind == TypeKind::LIST) return subType->equals(other->subType);
        return true;
    }

    std::string getDescriptor() {
        if (kind == TypeKind::PRIMITIVE) {
            switch (base) {
            case BaseType::INT:    return "I";
            case BaseType::FLOAT:  return "F";
            case BaseType::BOOL:   return "Z";
            case BaseType::STRING: return "Ljava/lang/String;";
            default:               return "V";
            }
        }
        if (kind == TypeKind::LIST) {
            return "[" + subType->getDescriptor();
        }
        if (kind == TypeKind::UNKNOWN) return "V";
        return "V";
    }

    std::string toString() {
        if (kind == TypeKind::UNKNOWN) return "?";
        if (kind == TypeKind::LIST) return "[" + subType->toString();
        if (kind == TypeKind::PRIMITIVE) {
            if (base == BaseType::INT) return "Int";
            if (base == BaseType::FLOAT) return "Float";
            if (base == BaseType::BOOL) return "Bool";
            if (base == BaseType::STRING) return "String";
        }
        return getDescriptor();
    }
};
