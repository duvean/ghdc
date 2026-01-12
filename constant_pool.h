#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "jvm_spec.h"

struct ConstantPoolEntry {
    JvmConstantTag tag;
    std::string stringValue; // для Utf8
    int intValue;            // для Integer
    float floatValue;        // для Float
    int refIndex1;           // class_index / string_index
    int refIndex2;           // name_and_type_index
};

class ConstantPool {
private:
    std::vector<ConstantPoolEntry> entries;
    std::map<std::string, int> utf8Cache; // Чтобы не дублировать строки

public:
    ConstantPool() { entries.emplace_back(); }

    int addUtf8(const std::string& val);
    int addStringLiteral(const std::string& val);
    int addInteger(int val);
    int addFloat(float val);

    const std::vector<ConstantPoolEntry>& getEntries() const { return entries; }
};