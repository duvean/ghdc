#include "class_builder.h"
#include "code_generator.h"

bool ClassBuilder::build() {
    // 1. Сначала добавляем UTF-8 строки для имен и дескрипторов ВСЕХ методов
    // Это гарантирует, что nameIdx и descIdx будут > 0
    for (auto& method : jvmClass->methods) {
        method.nameIdx = jvmClass->cp.addUtf8(method.name);
        method.descIdx = jvmClass->cp.addUtf8(method.descriptor);
    }

    // 2. Генерируем байткод (наполняем пул MethodRef-ами)
    for (auto& method : jvmClass->methods) {
        if (method.name != "main" && method.name != "<init>") {
            std::vector<uint8_t> bc;
            CodeGenerator gen(jvmClass->cp, bc);
            gen.generateFullMethod(method);
            method.bytecode = bc;
        }
    }

    // 3. Генерируем нативный main (теперь haskellMain точно есть в пуле)
    for (auto& method : jvmClass->methods) {
        if (method.name == "main") {
            int targetIdx = jvmClass->cp.addMethodRef(jvmClass->className, "haskellMain", "()V");
            method.bytecode = { 
                0xB8, (uint8_t)(targetIdx >> 8), (uint8_t)(targetIdx & 0xFF), // invokestatic
                0xB1                                                        // return
            };
        }
    }

    // 3. Теперь, когда пул констант полностью укомплектован, открываем файл
    out.open(filename, std::ios::binary);
    if (!out.is_open()) return false;

    writeU4(0xCAFEBABE);
    writeU2(0);  // Minor
    writeU2(49); // Major

    // Теперь это запишет ПОЛНЫЙ пул констант, включая tail и cons
    writeConstantPool(); 

    writeU2(0x0021); // Access Flags
    writeU2(static_cast<uint16_t>(jvmClass->classIdx));
    writeU2(static_cast<uint16_t>(jvmClass->superIdx));
    writeU2(0); // Interfaces
    writeU2(0); // Fields

    // 3. ЗАПИСЬ МЕТОДОВ
    // Теперь writeMethods должен просто брать уже готовый method.bytecode
    writeMethods();

    writeU2(0); // Attributes
    out.close();
    return true;
}

void ClassBuilder::prepareConstants() {
    // Убеждаемся, что базовые вещи на месте
    jvmClass->classIdx = jvmClass->cp.addClass(jvmClass->className);
    jvmClass->superIdx = jvmClass->cp.addClass("java/lang/Object");
    jvmClass->cp.addUtf8("Code"); // Индекс атрибута Code

    // Синхронизируем каждый метод
    for (auto& method : jvmClass->methods) {
        method.syncWithPool(jvmClass->cp);
    }
}

void ClassBuilder::writeConstantPool() {
    const auto& entries = jvmClass->cp.getEntries();
    
    // 1. Количество констант (размер вектора, т.к. 0-й элемент уже есть)
    writeU2(static_cast<uint16_t>(entries.size()));

    // 2. Сами константы (начинаем с 1, т.к. 0 зарезервирован)
    for (size_t i = 1; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        writeU1(static_cast<uint8_t>(entry.tag));

        switch (entry.tag) {
            case CONSTANT_Utf8: // Tag(1) + Len(2) + Bytes
                writeU2(static_cast<uint16_t>(entry.stringValue.size()));
                writeString(entry.stringValue);
                break;
            
            case CONSTANT_Integer: // Tag(3) + Bytes(4)
                writeU4(static_cast<uint32_t>(entry.intValue));
                break;
            
            case CONSTANT_Float: { // Tag(4) + Bytes(4)
                // Кастуем биты float в int для записи
                uint32_t intVal;
                std::memcpy(&intVal, &entry.floatValue, sizeof(float));
                writeU4(intVal);
                break;
            }

            case CONSTANT_Class: // Tag(7) + NameIdx(2)
                writeU2(static_cast<uint16_t>(entry.refIndex1));
                break;

            case CONSTANT_String: // Tag(8) + StringIdx(2)
                writeU2(static_cast<uint16_t>(entry.refIndex1));
                break;

            case CONSTANT_Fieldref:
            case CONSTANT_Methodref: // Tag(10) + ClassIdx(2) + NameTypeIdx(2)
                writeU2(static_cast<uint16_t>(entry.refIndex1));
                writeU2(static_cast<uint16_t>(entry.refIndex2));
                break;

            case CONSTANT_NameAndType: // Tag(12) + NameIdx(2) + DescIdx(2)
                writeU2(static_cast<uint16_t>(entry.refIndex1));
                writeU2(static_cast<uint16_t>(entry.refIndex2));
                break;

            default:
                std::cerr << "[ClassBuilder] Unknown constant tag: " << (int)entry.tag << "\n";
        }
    }
}

void ClassBuilder::writeMethods() {
    writeU2(static_cast<uint16_t>(jvmClass->methods.size()));

    for (const auto& method : jvmClass->methods) {
        writeU2(static_cast<uint16_t>(method.accessFlags));
        writeU2(static_cast<uint16_t>(method.nameIdx));
        writeU2(static_cast<uint16_t>(method.descIdx));
        writeU2(1); // Attributes Count

        writeU2(1); // Name "Code"

        std::vector<uint8_t> bytecode;
        if (method.name == "<init>") {
            bytecode = { 0x2A, 0xB7, 0x00, 0x09, 0xB1 };
        } else if (method.name == "main" && method.descriptor.find("([L") != std::string::npos) {
            int targetRef = jvmClass->cp.addMethodRef(jvmClass->className, "haskellMain", "()V");
            bytecode = { 0xB8, (uint8_t)(targetRef >> 8), (uint8_t)(targetRef & 0xFF), 0xB1 };
        } else {
            bytecode = method.bytecode;
        }

        uint32_t codeAttrLen = 2 + 2 + 4 + (uint32_t)bytecode.size() + 2 + 2;
        writeU4(codeAttrLen);
        writeU2(20); // max_stack
        writeU2(20); // max_locals
        writeU4((uint32_t)bytecode.size());
        writeBytes(bytecode);
        writeU2(0); // exceptions
        writeU2(0); // attributes
    }
}

uint8_t ClassBuilder::getReturnOpcode(const std::string& descriptor) {
    size_t parenIndex = descriptor.find(')');
    if (parenIndex == std::string::npos || parenIndex + 1 >= descriptor.size()) {
        return 0xB1; // На всякий случай
    }

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
        default:  return 0xB1;
    }
}
