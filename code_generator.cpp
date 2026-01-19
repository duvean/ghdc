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
            if (node->inferredType->base == BaseType::INT || node->inferredType->base == BaseType::BOOL) {
                // Если значение "True"/"False", мы уже положили 1/0 в константы или можем распарсить
                int val = 0;
                if (node->value == "True") val = 1;
                else if (node->value == "False") val = 0;
                else val = std::stoi(node->value);

                emit.iconst(val);
            }
            else if (node->inferredType->base == BaseType::FLOAT) {
                emit.emitU1(LDC, (uint8_t)node->constPoolIndex);
            }
            else if (node->inferredType->base == BaseType::STRING) {
                emit.emitU1(LDC, (uint8_t)node->constPoolIndex);
            }
            break;

        case NodeType::EXPR_VAR:
            if (node->isBuiltinFunciton) {
                // Это вызов статического метода без аргументов
                // constPoolIndex уже заполнен на семантике
                emit.emitU2(Opcode::INVOKESTATIC, (uint16_t)node->constPoolIndex);
            }
            else if (node->localVarIndex != -1) {
                uint8_t op = getLoadOpcode(node->inferredType);
                emit.emitU1(op, (uint8_t)node->localVarIndex);
            }
            break;

        case NodeType::EXPR_FUNC_CALL: {
            for (auto *arg : node->flattenedArgs) generateExpr(arg);
            emit.emitU2(INVOKESTATIC, (uint16_t)node->constPoolIndex);
            break;
        }

        case NodeType::EXPR_BINARY: {
            // Доступ к элементу массива
            if (node->op == "!!") {
                generateExpr(node->left);  // На стек: Ссылка на массив (Ref)
                generateExpr(node->right); // На стек: Индекс (Int)

                SemanticType* resType = node->inferredType;
                if (resType->base == BaseType::INT) 
                    emit.emit(IALOAD);
                else if (resType->base == BaseType::FLOAT) 
                    emit.emit(FALOAD);
                else 
                    emit.emit(AALOAD);
                return;
            }

            // Логические
            else if (node->op == "&&") {
                generateExpr(node->left);
                generateExpr(node->right);
                emit.emit(IAND);
                return;
            }
            else if (node->op == "||") {
                generateExpr(node->left);
                generateExpr(node->right);
                emit.emit(IOR);
                return;
            }

            // Сравнения
            else if (node->op == "==" || node->op == "<" || node->op == ">" || 
                    node->op == "<=" || node->op == "!=" || node->op == ">=") {
                
                generateExpr(node->left);
                generateExpr(node->right);

                bool isFloat = (node->left->inferredType->base == BaseType::FLOAT);
                
                if (isFloat) {
                    emit.emit(FCMPG); // stack: -1, 0, 1
                }

                // Выбираем инструкцию перехода
                uint8_t opcode;
                if (isFloat) {
                    // Сравнение результата FCMPG с нулем
                    if      (node->op == "==") opcode = IFEQ; // 0 == 0
                    else if (node->op == "!=") opcode = IFNE; // != 0
                    else if (node->op == "<")  opcode = IFLT; // -1 < 0
                    else if (node->op == ">")  opcode = IFGT; // 1 > 0
                    else if (node->op == "<=") opcode = IFLE; // -1 or 0
                    else                       opcode = IFGE; // 1 or 0 (>=)
                } else {
                    // INT сравнения (stack: v1, v2 -> jump)
                    if      (node->op == "==") opcode = IF_ICMPEQ;
                    else if (node->op == "!=") opcode = IF_ICMPNE;
                    else if (node->op == "<")  opcode = IF_ICMPLT;
                    else if (node->op == ">")  opcode = IF_ICMPGT;
                    else if (node->op == "<=") opcode = IF_ICMPLE;
                    else                       opcode = IF_ICMPGE;
                }

                // Генерация ветвления (True/False -> 1/0)
                size_t jumpOpAddr = emit.getCurrentOffset(); 
                emit.emitU2(opcode, 0); 
                size_t patchAddr = jumpOpAddr + 1;

                // Ветка FALSE
                emit.iconst(0);
                size_t gotoOpAddr = emit.getCurrentOffset();
                emit.emitU2(GOTO, 0);
                size_t gotoPatchAddr = gotoOpAddr + 1;

                // Ветка TRUE
                size_t trueTarget = emit.getCurrentOffset();
                emit.patchU2(patchAddr, (uint16_t)(trueTarget - jumpOpAddr));
                emit.iconst(1);

                // Конец
                size_t endTarget = emit.getCurrentOffset();
                emit.patchU2(gotoPatchAddr, (uint16_t)(endTarget - gotoOpAddr));
                return;
            }

            // Стандартная арифметика: сначала генерируем оба операнда
            generateExpr(node->left);
            generateExpr(node->right);

            if (node->inferredType->base == BaseType::FLOAT) {
                if      (node->op == "+") emit.emit(Opcode::FADD);
                else if (node->op == "-") emit.emit(Opcode::FSUB);
                else if (node->op == "*") emit.emit(Opcode::FMUL);
                else if (node->op == "/") emit.emit(Opcode::FDIV);
            } 
            else { // INT
                if      (node->op == "+") emit.emit(Opcode::IADD);
                else if (node->op == "-") emit.emit(Opcode::ISUB);
                else if (node->op == "*") emit.emit(Opcode::IMUL);
                else if (node->op == "/") emit.emit(Opcode::IDIV);
            }
            break;
        }

        case NodeType::EXPR_IF: {
            // 1. Генерируем условие (оставляет 0 или 1 на стеке)
            generateExpr(node->cond);

            // 2. IFEQ: если 0, прыгаем в ELSE
            size_t ifOpAddr = emit.getCurrentOffset(); // Запоминаем адрес начала инструкции
            emit.emitU2(IFEQ, 0);
            size_t ifPatchAddr = ifOpAddr + 1; // Адрес, куда писать смещение (сразу после опкода)

            // 3. Ветка THEN
            generateExpr(node->expr_true);
            
            // GOTO: после THEN нужно прыгнуть в самый конец (пропустить ELSE)
            size_t gotoOpAddr = emit.getCurrentOffset();
            emit.emitU2(GOTO, 0);
            size_t gotoPatchAddr = gotoOpAddr + 1;

            // 4. Ветка ELSE (начинается прямо здесь)
            size_t elseTargetAddr = emit.getCurrentOffset();
            // Патчим IFEQ: (адрес ELSE) - (адрес самого IFEQ)
            int16_t elseOffset = (int16_t)(elseTargetAddr - ifOpAddr);
            emit.patchU2(ifPatchAddr, (uint16_t)elseOffset);
            
            generateExpr(node->expr_false);

            // 5. КОНЕЦ (сюда прыгнет GOTO)
            size_t endTargetAddr = emit.getCurrentOffset();
            // Патчим GOTO: (адрес END) - (адрес самого GOTO)
            int16_t endOffset = (int16_t)(endTargetAddr - gotoOpAddr);
            emit.patchU2(gotoPatchAddr, (uint16_t)endOffset);
            
            break;
        }

        case NodeType::EXPR_ARRAY: {
            // 1. Кладем размер массива на стек
            int size = node->block.size();
            emit.iconst(size);

            // 2. Создаем массив
            SemanticType* elemType = node->inferredType->subType;
                
            if (elemType->base == BaseType::INT) {
                emit.emitU1(NEWARRAY, 10); // 10 = T_INT
            } else if (elemType->base == BaseType::FLOAT) {
                emit.emitU1(NEWARRAY, 6);  // 6 = T_FLOAT
            } else if (elemType->base == BaseType::STRING) {
                emit.emitU2(ANEWARRAY, (uint16_t)node->constPoolIndex);
            }

            // Сейчас пустой массив (ссылка) лежит на стеке.
                
            // 3. Заполняем элементами
            for (int i = 0; i < size; ++i) {
                emit.emit(DUP); // Дублируем ссылку на массив
                emit.iconst(i); // Индекс
                generateExpr(node->block[i]); // Значение элемента
                    
                // Сохраняем (Array Store)
                if (elemType->base == BaseType::INT) emit.emit(IASTORE);
                else if (elemType->base == BaseType::FLOAT) emit.emit(FASTORE);
                else emit.emit(AASTORE); // Для String
            }
            // На стеке осталась ссылка на заполненный массив
            break;
        }

        case NodeType::EXPR_DO_BLOCK: {
            if (node->decls) {
                for (auto *decl : node->decls->decls) {
                    generateDecl(decl);
                }
            }
            break;
        }

        case NodeType::EXPR_CASTING: {
            // Генерируем выражение, которое нужно скастить (например, Int)
            generateExpr(node->left);

            if (node->inferredType->base == BaseType::FLOAT && 
                node->left->inferredType->base == BaseType::INT) {
                emit.emit(Opcode::I2F);
            }
            break;
        }
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

        case NodeType::DECL_FUNC: {
            if (node->expr) {
                generateExpr(node->expr); // Результат тела функции на стеке
                    
                SemanticType* retType = node->expr->inferredType;
                if (retType->base == BaseType::IO || retType->base == BaseType::VOID) {
                    emit.emit(RETURN); // RETURN (void)
                } else if (retType->base == BaseType::INT || retType->base == BaseType::BOOL) {
                    emit.emit(IRETURN); // IRETURN
                } else if (retType->base == BaseType::FLOAT) {
                    emit.emit(FRETURN); // FRETURN
                } else {
                    emit.emit(ARETURN); // ARETURN (для объектов и массивов)
                }
            }
            break;
        }

        case NodeType::DECL_MONADIC_BIND: {
            if (node->expr) {
                // 1. Генерируем выражение (это вызовет INVOKESTATIC read*)
                // На стеке окажется результат ввода (Int, Float или ссылка)
                generateExpr(node->expr);

                // 2. Сохраняем в локальную переменную
                uint8_t op = getStoreOpcode(node->inferredType);
                emit.emitU1(op, (uint8_t)node->localVarIndex);
            }
            break;
        }

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
