#include "code_generator.h"

void CodeGenerator::generate(JvmMethod &method)
{
    // Если это синтетический метод (main или <init>), 
    // его байткод либо уже задан, либо запишется в ClassBuilder
    // Просто выходим
    if (method.bodies.empty() || (method.bodies.size() == 1 && method.bodies[0] == nullptr)) {
        return;
    }

    for (auto* bodyNode : method.bodies) {
        if (bodyNode) {
            generateDecl(bodyNode);
        }
    }
    // В конце каждого метода должен быть return,
    // если он не был добавлен явно
    if (method.bytecode.empty() || method.bytecode.back() != RETURN) {
        emit.emit(getReturnOpcode(method.descriptor));
    }
}

void CodeGenerator::generateExpr(ExprNode *node) {
    if (!node) return;

    switch (node->type) {
        case NodeType::EXPR_LITERAL:
            if (node->inferredType->base == BaseType::INT) {
                emit.iconst(std::stoi(node->value));
            }
            else if (node->inferredType->base == BaseType::STRING) {
                emit.emitU1(LDC, (uint8_t)node->constPoolIndex);
            }
            break;

        case NodeType::EXPR_VAR:
            if (node->localVarIndex != -1) {
                uint8_t op = getLoadOpcode(node->inferredType);
                emit.emitU1(op, (uint8_t)node->localVarIndex);
            }
            break;

        case NodeType::EXPR_FUNC_CALL:
            // 1. Генерируем аргументы (они ложатся на стек)
            for (auto *arg : node->arguments) 
                generateExpr(arg);

            // 2. Вызываем метод
            emit.emitU2(INVOKESTATIC, (uint16_t)node->constPoolIndex);
            break;

        case NodeType::EXPR_BINARY:
            generateExpr(node->left);
            generateExpr(node->right);
            if (node->op == "+")
                emit.emit(IADD);
            if (node->op == "-")
                emit.emit(ISUB);
            break;

        case NodeType::EXPR_DO_BLOCK:
            if (node->decls) {
                for (auto *decl : node->decls->decls) {
                    generateDecl(decl);
                }
            }
            break;
    }
}

void CodeGenerator::generateDecl(DeclNode *node) {
    if (!node) return;

    switch (node->type) {
        case NodeType::DECL_VAR:
            if (node->expr) {
                generateExpr(node->expr); 
                uint8_t op = getStoreOpcode(node->inferredType);
                emit.emitU1(op, (uint8_t)node->localVarIndex);
            }
            break;

        case NodeType::DECL_BLOCK:
            if (node->letBlock) {
                auto* list = dynamic_cast<DeclListNode*>(node->letBlock);
                if (list) {
                    for (auto* varDecl : list->decls) {
                        generateDecl(varDecl);
                    }
                }
            }
            break;

        case NodeType::DECL_FUNC:
            if (node->expr) generateExpr(node->expr);
            break;

        case NodeType::DECL_ACTION:
            // Это просто вызов функции (например, putStrLn)
            generateExpr(node->expr);
            // Если функция что-то возвращает (не void), в JVM это надо снять со стека
            // Но putStrLn возвращает void (V), так что POP не нужен.
            break;
            
        default:
            if (node->expr) generateExpr(node->expr);
            break;
    }
}

uint8_t CodeGenerator::getStoreOpcode(SemanticType* type) {
    // Если это список или объект
    if (type->kind == TypeKind::LIST || type->base == BaseType::STRING) {
        return ASTORE;
    }
    if (type->base == BaseType::FLOAT) {
        return FSTORE;
    }
    // По умолчанию для Int, Bool и прочих 32-битных типов
    return ISTORE;
}

uint8_t CodeGenerator::getLoadOpcode(SemanticType* type) {
    if (type->kind == TypeKind::LIST || type->base == BaseType::STRING) {
        return ALOAD;
    }
    if (type->base == BaseType::FLOAT) {
        return FLOAD;
    }
    return ILOAD;
}

uint8_t CodeGenerator::getReturnOpcode(const std::string &descriptor)
{
    size_t parenIndex = descriptor.find(')');
    if (parenIndex == std::string::npos || parenIndex + 1 >= descriptor.size())
        return 0xB1; // На всякий случай

    char returnStart = descriptor[parenIndex + 1];

    switch (returnStart) {
        case 'V': return 0xB1; // RETURN (void)
        case '[': return 0xB0; // ARETURN (любой массив: [I, [F, [L...;)
        case 'L': return 0xB0; // ARETURN (объект)
        case 'I':
        case 'Z':
        case 'B':
        case 'C':
        case 'S': return 0xAC; // IRETURN (целые и bool)
        case 'F': return 0xAE; // FRETURN (float)
        case 'D': return 0xAF; // DRETURN (double)
        case 'J': return 0xAD; // LRETURN (long)
        default: return 0xB1;
    }
}
