#pragma once
#include <vector>
#include <cstdint>

enum Opcode : uint8_t {
    ICONST_0 = 0x03, ICONST_1 = 0x04,
    BIPUSH   = 0x10, LDC      = 0x12,
    ILOAD    = 0x15, ISTORE   = 0x36,
    FLOAD    = 0x17, FSTORE   = 0x38,
    ALOAD    = 0x19, ASTORE   = 0x3A,
    IADD     = 0x60, ISUB     = 0x64,
    FADD     = 0x62, FSUB     = 0x66,
    IMUL     = 0x68, IDIV     = 0x6C,   
    FMUL     = 0x6A, FDIV     = 0x6E,
    IRETURN  = 0xAC, ARETURN  = 0xB0, 
    RETURN   = 0xB1, FRETURN  = 0xAE,
    INVOKESTATIC = 0xB8, I2F  = 0x86
};

class BytecodeEmitter {
    std::vector<uint8_t>& code;
public:
    BytecodeEmitter(std::vector<uint8_t>& out) : code(out) {}

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
    
    // Умные обертки
    void iconst(int val) {
        if (val >= 0 && val <= 5) emit(0x03 + val);
        else emitU1(BIPUSH, (uint8_t)val); 
    }
};