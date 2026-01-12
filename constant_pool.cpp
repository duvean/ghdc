#include "constant_pool.h"

int ConstantPool::addUtf8(const std::string& val) {
        if (utf8Cache.find(val) != utf8Cache.end()) {
            return utf8Cache[val];
        }
        ConstantPoolEntry e;
        e.tag = CONSTANT_Utf8;
        e.stringValue = val;
        entries.push_back(e);
        int idx = (int)entries.size() - 1;
        utf8Cache[val] = idx;
        return idx;
    }

    // Добавляет строковый литерал (CONSTANT_String -> CONSTANT_Utf8)
int ConstantPool::addStringLiteral(const std::string& val) {
        int utf8Index = addUtf8(val);
        ConstantPoolEntry e;
        e.tag = CONSTANT_String;
        e.refIndex1 = utf8Index;
        entries.push_back(e);
        return (int)entries.size() - 1;
    }

int ConstantPool::addInteger(int val) {
        // Integer не кешируем для простоты (хотя можно)
        ConstantPoolEntry e;
        e.tag = CONSTANT_Integer;
        e.intValue = val;
        entries.push_back(e);
        return (int)entries.size() - 1;
    }

int ConstantPool::addFloat(float val) {
        ConstantPoolEntry e;
        e.tag = CONSTANT_Float;
        e.floatValue = val;
        entries.push_back(e);
        return (int)entries.size() - 1;
    }