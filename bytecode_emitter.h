#pragma once
#include <vector>
#include <cstdint>

enum Opcode : uint8_t {
    ICONST_0 = 0x03, ICONST_1  = 0x04,
    BIPUSH   = 0x10, LDC       = 0x12,
    ILOAD    = 0x15, ISTORE    = 0x36,
    FLOAD    = 0x17, FSTORE    = 0x38,
    ALOAD    = 0x19, ASTORE    = 0x3A,
    IADD     = 0x60, ISUB      = 0x64,
    FADD     = 0x62, FSUB      = 0x66,
    IMUL     = 0x68, IDIV      = 0x6C,   
    FMUL     = 0x6A, FDIV      = 0x6E,
    IRETURN  = 0xAC, ARETURN   = 0xB0, 
    RETURN   = 0xB1, FRETURN   = 0xAE,
    DUP      = 0x59, I2F       = 0x86,
    NEWARRAY = 0xBC, ANEWARRAY = 0xBD,
    IASTORE  = 0x4F, AASTORE   = 0x53,
    FASTORE  = 0x51,
    IALOAD = 0x2E, FALOAD = 0x30, AALOAD = 0x32,
    INVOKESTATIC = 0xB8,
    FCMPG = 0x96, IFEQ = 0x99, IFLT = 0x9B, IFGT = 0x9D, IFLE = 0x9E, GOTO = 0xA7,
    IF_ICMPEQ = 0x9F, IF_ICMPLT = 0xA1, IF_ICMPGT = 0xA3, IF_ICMPLE = 0xA4
};

class BytecodeEmitter {
    std::vector<uint8_t>& code;
public:
    BytecodeEmitter(std::vector<uint8_t>& out) : code(out) {}

    // Возвращает текущую позицию (адрес следующего байта)
    size_t getCurrentOffset() const { 
        return code.size(); 
    }

    void emit(uint8_t opcode) { code.push_back(opcode); }
    
    // Для команд с 1 аргументом (ldc, bipush, iload_slot)
    void emitU1(uint8_t opcode, uint8_t arg) {
        code.push_back(opcode);
        code.push_back(arg);
    }

    // Для команд с 2 аргументами (invokestatic idx, и т.д.)
    void emitU2(uint8_t opcode, uint16_t arg) {
        code.push_back(opcode);
        code.push_back((arg >> 8) & 0xFF);
        code.push_back(arg & 0xFF);
    }
    
    // Позволяет пропатчить 2 байта по конкретному адресу
    void patchU2(size_t pos, uint16_t value) {
        if (pos + 1 < code.size()) {
            code[pos] = (value >> 8) & 0xFF;
            code[pos + 1] = value & 0xFF;
        }
    }

    void iconst(int val) {
        if (val >= 0 && val <= 5) emit(0x03 + val);
        else if (val >= -128 && val <= 127) emitU1(0x10, (uint8_t)val); // BIPUSH
        else emitU2(0x11, (uint16_t)val); // SIPUSH (или LDC если больше 32767)
    }
};