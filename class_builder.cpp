#include "class_builder.h"

bool ClassBuilder::build() {
    prepareConstants();

    out.open(filename, std::ios::binary);
    if (!out.is_open()) return false;

    // 1. Magic Number (0xCAFEBABE)
    writeU4(0xCAFEBABE);

    // 2. Version (Java 8 = 52.0 -> Major 52, Minor 0)
    writeU2(0);  // Minor
    writeU2(52); // Major

    // 3. Constant Pool
    writeConstantPool();

    // 4. Access Flags
    // ACC_PUBLIC (0x0001) + ACC_SUPER (0x0020) = 0x0021
    writeU2(0x0021);

    // 5. This Class Index (#3 в вашем примере)
    writeU2(static_cast<uint16_t>(jvmClass->classIdx));

    // 6. Super Class Index (#5 в вашем примере)
    writeU2(static_cast<uint16_t>(jvmClass->superIdx));

    // 7. Interfaces Count (0)
    writeU2(0);

    // 8. Fields Count (0)
    writeU2(0);

    // 9. Methods
    writeMethods();

    // 10. Class Attributes (SourceFile и т.д.) - пока 0
    writeU2(0);

    out.close();
    std::cout << "[ClassBuilder] Successfully wrote " << filename << "\n";
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
    // Количество методов
    writeU2(static_cast<uint16_t>(jvmClass->methods.size()));

    for (const auto& method : jvmClass->methods) {
        writeU2(static_cast<uint16_t>(method.accessFlags));
        writeU2(static_cast<uint16_t>(method.nameIdx));
        writeU2(static_cast<uint16_t>(method.descIdx));

        // --- ATTRIBUTES (Code) ---
        // Каждый неабстрактный метод должен иметь атрибут "Code"
        writeU2(1); // Attributes Count = 1 (только Code)

        // 1. Ищем индекс строки "Code" в Constant Pool
        int codeNameIdx = 1; // Он всегда #1 хардкодом но можно сделать поиск на всякий

        writeU2(codeNameIdx); // Attribute Name Index

        // --- ГЕНЕРАЦИЯ БАЙТКОДА (Заглушка) ---
        std::vector<uint8_t> bytecode;
        
        if (method.name == "<init>") {
            // Оставляем вашу ручную логику для конструктора
            bytecode = { 0x2A, 0xB7, 0x00, 0x09, 0xB1 }; 
        } 
        else if (method.name == "main" && method.descriptor.find("([L") != std::string::npos) {
            // Оставляем синтетический main (вызов haskellMain)
            int targetRef = jvmClass->cp.addMethodRef(jvmClass->className, "haskellMain", "()V");
            bytecode = { 
                0xB8, (uint8_t)(targetRef >> 8), (uint8_t)(targetRef & 0xFF), 
                0xB1 
            };
        } 
        else {
            // ДЛЯ ВСЕХ ОСТАЛЬНЫХ (haskellMain, insert, findMin):
            // Берем то, что сгенерировал CodeGenerator
            bytecode = method.bytecode;

            // Если ничего нет то поплачем и добавим ретёрн
            if (bytecode.empty()) {
                bytecode.push_back(getReturnOpcode(method.descriptor));
            }
        }

        // --- СЧИТАЕМ РАЗМЕР И ПИШЕМ ---
        uint32_t codeAttrLen = 2 + 2 + 4 + (uint32_t)bytecode.size() + 2 + 2;
        writeU4(codeAttrLen);
        writeU2(10); // max_stack
        writeU2(10); // max_locals
        writeU4((uint32_t)bytecode.size());
        writeBytes(bytecode);
        
        writeU2(0); // exception_table
        writeU2(0); // attributes_count (внутри Code)
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
