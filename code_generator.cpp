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
            if (node->isEnumConstant) {
                // 1. Сначала проверяем, есть ли уже индекс в узле
                int cpIdx = node->constPoolIndex;
                
                // 2. Если индекс 0 или не задан, создаем его заново в пуле
                if (cpIdx <= 0) {
                    int val = std::stoi(node->value);
                    cpIdx = cp.addInteger(val);
                }
                
                std::cout << "[DEBUG-GEN] LDC for " << node->name << " uses CP index: " << cpIdx << std::endl;
                // 3. Генерируем ldc
                emit.emitU1(LDC, (uint8_t)cpIdx);
            }
            else if (node->isBuiltinFunciton) { // Вызов метода без аргументов 
                emit.emitU2(INVOKESTATIC, (uint16_t)node->constPoolIndex);
            }
            else if (node->localVarIndex != -1) { // Обычная переменная, определяем opcode
                uint8_t op = getLoadOpcode(node->inferredType);
                emit.emitU1(op, (uint8_t)node->localVarIndex);
            }
            break;

        case NodeType::EXPR_FUNC_CALL: {
            if (node->flattenedArgs.empty()) return; 

            // 1. Находим базу функции
            ExprNode* baseFunc = node;
            while (baseFunc->function) baseFunc = baseFunc->function;
            std::string name = baseFunc->name;
            if (name.find(' ') != std::string::npos) name = name.substr(0, name.find(' '));

            bool isMap = (name == "map");
            bool isFold = (name == "fold");

            // === СПЕЦИАЛЬНАЯ ГЕНЕРАЦИЯ АРГУМЕНТОВ ДЛЯ MAP/FOLD ===
            if (isMap || isFold) {
                // Аргумент 0: Функция. Вместо generateExpr мы пушим имя класса и метода.
                ExprNode* funcArg = node->flattenedArgs[0];
                
                // Предполагаем, что funcArg - это EXPR_VAR (имя функции)
                // Имя класса пока хардкодим как HaskellProgram (или HaskellRuntime, если встроенная)
                std::string targetClass = funcArg->isBuiltinFunciton ? "HaskellRuntime" : "HaskellProgram";
                
                // 1. PUSH Class Name
                int classStrIdx = cp.addStringLiteral(targetClass);
                emit.emitU2(LDC_W, (uint16_t)classStrIdx);

                // 2. PUSH Method Name
                int methodStrIdx = cp.addStringLiteral(funcArg->name);
                emit.emitU2(LDC_W, (uint16_t)methodStrIdx);

                // 3. Остальные аргументы генерируем штатно
                for (size_t i = 1; i < node->flattenedArgs.size(); ++i) {
                    generateExpr(node->flattenedArgs[i]);
                }
            } else { // Генерируем код для всех аргументов
                for (auto* arg : node->flattenedArgs) generateExpr(arg);
            }

            // Проверка вложенности массива
            auto isComplexArray = [](SemanticType* type) -> bool {
                if (type->kind != TypeKind::LIST) return false;
                return type->subType->kind == TypeKind::LIST;
            };

            // 3. Формируем дескриптор и флаги
            std::string descriptor;
            bool needCheckCast = false; // Нужно ли приведение типов после вызова
            std::string targetClass = baseFunc->isBuiltinFunciton ? "HaskellRuntime" : "HaskellProgram";

            // Логика выбора дескриптора
            if (isMap) {
                // (ClassName, MethodName, List) -> List
                descriptor = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)Ljava/lang/Object;";
                needCheckCast = true; // Результат Object, нужно привести к реальному типу списка
            }
            else if (isFold) {
                SemanticType* accType = node->flattenedArgs[1]->inferredType;
                
                if (accType->base == BaseType::FLOAT) {
                    // Дескриптор с 'F' для аккумулятора
                    descriptor = "(Ljava/lang/String;Ljava/lang/String;FLjava/lang/Object;)Ljava/lang/Object;";
                } else {
                    // Дескриптор с 'I' для аккумулятора
                    descriptor = "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/Object;)Ljava/lang/Object;";
                }
                needCheckCast = true;
            }
            else if (baseFunc->isBuiltinFunciton && name != "print") {
                SemanticType* firstArgType = node->flattenedArgs[0]->inferredType;

                if (name == "head" || name == "last" || name == "tail" || name == "get") {
                    // Функции с одним аргументом-массивом
                    if (isComplexArray(firstArgType)) {
                        if (name == "tail") { // tail :: [a] -> [a]
                            descriptor = "([Ljava/lang/Object;)[Ljava/lang/Object;";
                        } else if (name == "get") { // get :: [a] -> Int -> a
                            descriptor = "([Ljava/lang/Object;I)Ljava/lang/Object;";
                        } else { // head/last :: [a] -> a
                            descriptor = "([Ljava/lang/Object;)Ljava/lang/Object;";
                        }
                        needCheckCast = true;
                    } else { // Для простых массивов берем сохраненную сигнатуру
                        descriptor = baseFunc->inferredType->getDescriptor();
                    }
                }
                else { // Другие встроенные функции    
                    descriptor = baseFunc->inferredType->getDescriptor();
                }
            } 
            else if (name == "print" && !node->flattenedArgs.empty()) {
                SemanticType* argType = node->flattenedArgs[0]->inferredType;
                
                if (isComplexArray(argType)) { // Для любого вложенного списка используем дескриптор Object[]
                    descriptor = "([Ljava/lang/Object;)V";
                } else { // Для плоских списков [Int] [Float] [String] оставляем как было
                    descriptor = "(" + argType->getDescriptor() + ")V";
                }
            }
            else { // Пользовательские функции
                descriptor = baseFunc->inferredType->getDescriptor();
            }

            // 4. Добавляем метод в CP и генерируем вызов
            int methodRef = cp.addMethodRef(targetClass, name, descriptor);
            emit.emitU2(INVOKESTATIC, (uint16_t)methodRef);

            // 5. Генерируем CHECKCAST
            if (needCheckCast) {
                if (node->inferredType->base == BaseType::INT) {
                    int classIdx = cp.addClass("java/lang/Integer");
                    emit.emitU2(CHECKCAST, (uint16_t)classIdx);
                    int methodRef = cp.addMethodRef("java/lang/Integer", "intValue", "()I");
                    emit.emitU2(INVOKEVIRTUAL, (uint16_t)methodRef);
                } 
                else if (node->inferredType->base == BaseType::FLOAT) {
                    int classIdx = cp.addClass("java/lang/Float");
                    emit.emitU2(CHECKCAST, (uint16_t)classIdx);
                    int methodRef = cp.addMethodRef("java/lang/Float", "floatValue", "()F");
                    emit.emitU2(INVOKEVIRTUAL, (uint16_t)methodRef);
                } 
                else { // Стандартная логика для массивов и других объектов
                    std::string castType = node->inferredType->getDescriptor();
                    if (castType.size() > 2 && castType.front() == 'L' && castType.back() == ';') {
                        castType = castType.substr(1, castType.size() - 2);
                    }
                    int classIdx = cp.addClass(castType);
                    emit.emitU2(CHECKCAST, (uint16_t)classIdx);
                }
            }
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

            // Cons (x : xs)
            if (node->op == ":") {
                // 1. Сначала кладём аргументы на стек
                generateExpr(node->left);  // Head
                generateExpr(node->right); // Tail
                
                // 2. Определяем типы для выбора метода
                SemanticType* headType = node->left->inferredType;
                SemanticType* tailType = node->right->inferredType;
                
                std::string descriptor;
                bool needCheckCast = false;

                // Проверка вложенности
                auto isComplex = [](SemanticType* t) {
                    return t && t->kind == TypeKind::LIST && t->subType && t->subType->kind == TypeKind::LIST;
                };

                // 3. Выбор дескриптора
                if (isComplex(tailType)) {
                    // Используем универсальный Object метод
                    descriptor = "(Ljava/lang/Object;[Ljava/lang/Object;)[Ljava/lang/Object;";
                    needCheckCast = true;
                } 
                else if (headType && headType->kind == TypeKind::PRIMITIVE && headType->base == BaseType::STRING) {
                    // Случай: String : [String]
                    descriptor = "(Ljava/lang/String;[Ljava/lang/String;)[Ljava/lang/String;";
                } 
                else {
                    // Случай: Int : [Int] или Float : [Float]
                    // Генерируем стандартный дескриптор: (тип_головы, тип_хвоста)тип_хвоста
                    descriptor = "(" + headType->getDescriptor() + tailType->getDescriptor() + ")" + tailType->getDescriptor();
                }

                // 4. Генерируем вызов
                int methodRef = cp.addMethodRef("HaskellRuntime", "cons", descriptor);
                emit.emitU2(INVOKESTATIC, (uint16_t)methodRef);

                // 5. Если работали через Object, возвращаем тип
                if (needCheckCast) {
                    std::string resultDesc = node->inferredType->getDescriptor();
                    int classIdx = cp.addClass(resultDesc);
                    emit.emitU2(CHECKCAST, (uint16_t)classIdx);
                }
                return;
            }

            // Логические
            else if (node->op == "&&") {
                // 1. Вычисляем левый операнд
                generateExpr(node->left); 

                // 2. Если False (0), прыгаем к метке "false_branch"
                size_t jumpToFalseAddr = emit.getCurrentOffset();
                emit.emitU2(IFEQ, 0); 

                // 3. Вычисляем правый операнд (только если левый был True)
                generateExpr(node->right);

                // 4. Прыгаем в самый конец (результат правого операнда остается на стеке)
                size_t skipFalseAddr = emit.getCurrentOffset();
                emit.emitU2(GOTO, 0);

                // 5. Метка False: сюда попадаем только если левый был 0
                size_t falseTarget = emit.getCurrentOffset();
                emit.patchU2(jumpToFalseAddr + 1, (uint16_t)(falseTarget - jumpToFalseAddr));
                emit.iconst(0);

                size_t endTarget = emit.getCurrentOffset();
                emit.patchU2(skipFalseAddr + 1, (uint16_t)(endTarget - skipFalseAddr));
                return;
            }
            else if (node->op == "||") {
                // 1. Вычисляем левый операнд
                generateExpr(node->left);

                // 2. Если True (не 0), прыгаем к метке "true_branch"
                size_t jumpToTrueAddr = emit.getCurrentOffset();
                emit.emitU2(IFNE, 0);

                // 3. Вычисляем правый операнд (только если левый был False)
                generateExpr(node->right);

                // 4. Прыгаем в самый конец (результат правого операнда остается на стеке)
                size_t skipTrueAddr = emit.getCurrentOffset();
                emit.emitU2(GOTO, 0);

                // 5. Метка True: сюда попадаем только если левый был 1
                size_t trueTarget = emit.getCurrentOffset();
                emit.patchU2(jumpToTrueAddr + 1, (uint16_t)(trueTarget - jumpToTrueAddr));
                emit.iconst(1);

                size_t endTarget = emit.getCurrentOffset();
                emit.patchU2(skipTrueAddr + 1, (uint16_t)(endTarget - skipTrueAddr));
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
                else if (node->op == "/") {                    
                    // 1. Конвертируем второй операнд (он сверху) в Float
                    emit.emit(Opcode::I2F); 
                    // 2. Временно сохраняем его или меняем местами, чтобы конвертировать первый
                    emit.emit(Opcode::SWAP); 
                    // 3. Конвертируем первый операнд в Float
                    emit.emit(Opcode::I2F);
                    // 4. Снова меняем местами, чтобы порядок был [val1_float, val2_float]
                    emit.emit(Opcode::SWAP);
                    // 5. Выполняем дробное деление
                    emit.emit(Opcode::FDIV);
                }
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
            int size = node->block.size();
            emit.iconst(size);

            SemanticType* elemType = node->inferredType->subType;
            std::string desc = elemType->getDescriptor();

            if (desc == "I") {
                emit.emitU1(NEWARRAY, 10); // T_INT
            } else if (desc == "F") {
                emit.emitU1(NEWARRAY, 6);  // T_FLOAT
            } else if (desc == "Z") {
                emit.emitU1(NEWARRAY, 4);  // T_BOOLEAN
            } else {
                // Обрезание (L и последней ;)
                if (desc[0] == 'L') {    
                    desc = desc.substr(1, desc.size() - 2);
                }
                // Если это массив массивов [[I] или массив объектов
                int classIdx = cp.addClass(desc); 
                //emit.emitU1(NEWARRAY, 10);
                emit.emitU2(ANEWARRAY, (uint16_t)classIdx);
            }

            // Если массив пустой, цикл просто не выполнится, 
            // и на стеке останется ссылка на пустой массив.
            for (int i = 0; i < size; ++i) {
                emit.emit(DUP); 
                emit.iconst(i); 
                generateExpr(node->block[i]); 
                    
                if (elemType->base == BaseType::INT) emit.emit(IASTORE);
                else if (elemType->base == BaseType::FLOAT) emit.emit(FASTORE);
                else emit.emit(AASTORE);
            }
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
            size_t patchPos = 0;
            size_t opcodePos = 0;

            if (node->paramsList) {
                auto* params = static_cast<DeclListNode*>(node->paramsList);
                for (size_t i = 0; i < params->decls.size(); ++i) {
                    auto* varDecl = params->decls[i];
                    auto* pattern = static_cast<ExprNode*>(varDecl->expr);

                    if (pattern && pattern->type == NodeType::EXPR_PATTERN_CONSTRUCTOR) {
                        std::cout << "[DEBUG] Pattern Name: " << pattern->name << std::endl;
                        std::cout << "[DEBUG] Pattern Value String: '" << pattern->value << "'" << std::endl;
                        // 1. Загружаем аргумент
                        emit.emitU1(ILOAD, (uint8_t)i);

                        // 2. Загружаем значение константы
                        // Используем stoi, тк в ExprNode.value лежит строка
                        int val = 0;
                        try {
                            if (pattern->value.empty()) {
                                // Если в value пусто, возможно мы забыли прокинуть enumValue из SymbolInfo
                                std::cout << "[WARN] Pattern value is empty, check semantic analysis!" << std::endl;
                            } else {
                                val = std::stoi(pattern->value);
                            }
                        } catch (const std::exception& e) {
                            std::cout << "[ERROR] stoi failed for value: " << pattern->value << std::endl;
                            // В качестве фоллбека, если мы не прокинули число, 
                            // но у нас есть enumValue в узле (если ты его добавлял):
                            // val = pattern->enumValue; 
                        }
                        emitIConst(val);

                        // 3. Заглушка для последующего патча
                        opcodePos = emit.getCurrentOffset(); // Запоминаем где стоит 0xA0
                        emit.emit(IF_ICMPNE);                // Пишем опкод
                        patchPos = emit.getCurrentOffset();  // Запоминаем где начнутся 00 00
                        emit.emitRawU2(0);                   // Пишем заглушку
                    }
                }
            }

            // 4. Тело функции
            if (node->expr) {
                generateExpr(node->expr);

                SemanticType* retType = node->expr->inferredType;
                if (retType->base == BaseType::IO || retType->base == BaseType::VOID) {
                    emit.emit(RETURN);
                } else if (retType->base == BaseType::INT || retType->base == BaseType::BOOL || 
                        (retType->kind == TypeKind::CONSTRUCTOR && retType->typeName != "IO")) {
                    emit.emit(IRETURN);
                } else if (retType->base == BaseType::FLOAT) {
                    emit.emit(FRETURN);
                } else {
                    emit.emit(ARETURN);
                }
            }

            // 5. Пришиваем патч
            if (patchPos != 0) {
                size_t currentPos = emit.getCurrentOffset();
                // Смещение = Текущая позиция (конец тела) - Позиция начала инструкции IF_ICMPNE
                int16_t offset = static_cast<int16_t>(currentPos - opcodePos);
                emit.patchU2(patchPos, static_cast<uint16_t>(offset));
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

// Возвращает адрес (offset) инструкции GOTO, которую нужно пропатчить, 
// если паттерн НЕ совпал (переход к следующему телу)
std::vector<size_t> CodeGenerator::generatePatternChecks(ASTNode* paramsList) {
    std::vector<size_t> failJumps;
    if (!paramsList) return failJumps;

    // Приводим к списку узлов (DeclListNode или как он у тебя назван)
    auto* declList = dynamic_cast<DeclListNode*>(paramsList);
    if (!declList) return failJumps;

    for (size_t i = 0; i < declList->decls.size(); ++i) {
        // Каждый параметр функции - это VarDeclNode
        auto* varDecl = dynamic_cast<DeclNode*>(declList->decls[i]);
        if (varDecl && varDecl->expr) {
            // Вот тут мы достаем сам паттерн (PATT_VAR, PATT_CONS и т.д.)
            size_t patchAddr = matchPattern(dynamic_cast<ExprNode*>(varDecl->expr), (int)i);
            if (patchAddr > 0) failJumps.push_back(patchAddr);
        }
    }
    return failJumps;
}

size_t CodeGenerator::matchPattern(ExprNode* pattern, int argLocIdx) {
    if (!pattern) return 0;

    if (pattern->type == NodeType::EXPR_PATTERN_VAR) {
        // Если это просто 'insert x', то x уже лежит в argLocIdx.
        // Семантика должна была присвоить pattern->localVarIndex = argLocIdx.
        // Никакого кода генерировать не нужно.
        return 0; 
    }

    if (pattern->type == NodeType::EXPR_PATTERN_LIST) {
        emit.emitU1(ALOAD, (uint8_t)argLocIdx);
        emit.emit(ARRAYLENGTH);
        
        emit.emit(IFNE);
        size_t offsetAddr = emit.getCurrentOffset();
        emit.emitRawU2(0); // Резервируем 2 байта
        return offsetAddr;
    }

    if (pattern->type == NodeType::EXPR_PATTERN_CONSTRUCTOR) {
        // Загружаем аргумент
        emit.emitU1(ILOAD, (uint8_t)argLocIdx);

        // Загружаем ожидаемое значение
        int expectedVal = 0;
        try {
            expectedVal = std::stoi(pattern->value);
        } catch (...) {
            std::cerr << "[Gen Error] Bad enum value in pattern: " << pattern->name << "\n";
        }
        emitIConst(expectedVal);

        // Если не равны, прыгаем к следующему телу
        emit.emit(IF_ICMPNE);
        
        // Запоминаем позицию для патчинга
        size_t opcodeOffset = emit.getCurrentOffset(); 
        emit.emitRawU2(0); 

        // Возвращаем адрес инструкции (чтобы потом посчитать смещение от него)
        return opcodeOffset;
    }

    if (pattern->type == NodeType::EXPR_PATTERN_CONS) {
        // (x:xs) - проверяем длину > 0
        emit.emitU1(ALOAD, (uint8_t)argLocIdx);
        emit.emit(ARRAYLENGTH); 
        
        emit.emit(IFEQ);
        size_t offsetAddr = emit.getCurrentOffset();
        emit.emitRawU2(0);

        // Извлекаем x (Head)
        emit.emitU1(ALOAD, (uint8_t)argLocIdx); 
        emit.emit(ICONST_0);
        
        // Выбираем тип загрузки в зависимости от типа элемента
        if (pattern->left->inferredType->base == BaseType::FLOAT) {
            emit.emit(FALOAD);
            emit.emitU1(FSTORE, (uint8_t)pattern->left->localVarIndex);
        } else if (pattern->left->inferredType->base == BaseType::INT || pattern->left->inferredType->base == BaseType::BOOL) {
            emit.emit(IALOAD);
            emit.emitU1(ISTORE, (uint8_t)pattern->left->localVarIndex);
        } else {
            emit.emit(AALOAD);
            emit.emitU1(ASTORE, (uint8_t)pattern->left->localVarIndex);
        }

        // Извлекаем xs (Tail)
        // Генерируем правильный дескриптор для tail
        std::string tailDesc = "(" + pattern->inferredType->getDescriptor() + ")" + pattern->inferredType->getDescriptor();
        int tailRef = cp.addMethodRef("HaskellRuntime", "tail", tailDesc);
        
        emit.emitU1(0x19, (uint8_t)argLocIdx);
        emit.emitU2(INVOKESTATIC, (uint16_t)tailRef);
        emit.emitU1(ASTORE, (uint8_t)pattern->right->localVarIndex);

        return offsetAddr;
    }
    
    return 0;
}

void CodeGenerator::generateFullMethod(JvmMethod& method) {
    for (size_t i = 0; i < method.bodies.size(); ++i) {
        DeclNode* bodyDecl = method.bodies[i];
        std::vector<size_t> currentMatchFails = generatePatternChecks(bodyDecl->paramsList);
        
        generateExpr(bodyDecl->expr);
        emit.emit(getReturnOpcode(method.descriptor));

        size_t nextBodyStart = emit.getCurrentOffset();
        for (size_t patchAddr : currentMatchFails) {
            if (patchAddr > 0) {
                // patchAddr указывает на начало U2 смещения
                size_t opcodeAddr = patchAddr - 1; 
                int16_t relativeOffset = static_cast<int16_t>(nextBodyStart - opcodeAddr);
                emit.patchU2(patchAddr, static_cast<uint16_t>(relativeOffset));
            }
        }
    }
    emitDefaultReturn(method.descriptor);
}



uint8_t CodeGenerator::getStoreOpcode(SemanticType* type) {
    // Если это список или объект
    if (type->kind == TypeKind::LIST || type->base == BaseType::STRING) {
        return ASTORE;
    }
    if (type->base == BaseType::FLOAT) {
        return FSTORE;
    }
    return ISTORE;
}

uint8_t CodeGenerator::getLoadOpcode(SemanticType* type) {
    if (type->kind == TypeKind::LIST || type->base == BaseType::STRING) {
        return ALOAD;
    }
    if (type->kind == TypeKind::CONSTRUCTOR && type->typeName != "IO") {
        return ILOAD; 
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
        return RETURN; // На всякий случай

    char returnStart = descriptor[parenIndex + 1];

    switch (returnStart) {
        case 'V': return RETURN;  // (void)
        case '[': return ARETURN; // (любой массив: [I, [F, [L...;)
        case 'L': return ARETURN; // (объект)
        case 'I':
        case 'Z':
        case 'B':
        case 'C':
        case 'S': return IRETURN; // (целые и bool)
        case 'F': return FRETURN; // (float)
        default:  return RETURN;
    }
}

void CodeGenerator::emitDefaultReturn(const std::string& descriptor) {
    uint8_t op = getReturnOpcode(descriptor);
    
    if (op == IRETURN) { // Int, Bool
        emit.emit(ICONST_0);
    } 
    else if (op == FRETURN) { // Float
        emit.emit(FCONST_0);
    }
    else if (op == ARETURN) { // Массивы/Объекты
        emit.emit(ACONST_NULL);
    }
    // Если void (0xB1), ничего класть не нужно

    emit.emit(op);
}

void CodeGenerator::emitIConst(int val) {
    if (val >= 0 && val <= 5) {
        emit.emit(0x03 + val); // iconst_0 ... iconst_5
    } else if (val == -1) {
        emit.emit(ICONST_M1);
    } else if (val >= -128 && val <= 127) {
        emit.emitU1(BIPUSH, (uint8_t)val);
    } else {
        // Если значение большое, добавляем в Constant Pool
        int cpIdx = cp.addInteger(val);
        if (cpIdx <= 255) {
            emit.emitU1(LDC, (uint8_t)cpIdx);
        } else {
            emit.emitU2(LDC_W, (uint16_t)cpIdx);
        }
    }
}
