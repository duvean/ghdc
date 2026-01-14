#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "parser.tab.h"
#include "constant_pool.h"
#include "semantic_analyzer.h"

extern int yyparse();
extern FILE* yyin;
extern ProgramNode* root;

int main(int argc, char** argv) {
    if(argc > 1) {
        if(freopen(argv[1], "r", stdin) == NULL) {
            std::cerr << "Error: Cannot open file " << argv[1] << " for reading.\n";
            return(1);
        }
        yyin = stdin;
    }
    else {
        std::cerr << "Error: No input file specified. Usage: " << argv[0] << " <file.hs>\n";
        return(1);
    }

    int parse_result = yyparse();                                                                                                                                                                                                            
                                                                                                                                                                                                                  
    if(root) {
        // --- СЕМАНТИЧЕСКИЙ АНАЛИЗ ---
        std::cout << "--- Semantic Analysis Start ---\n";
        
        ConstantPool constPool;
        SemanticAnalyzer analyzer(constPool);
        
        analyzer.analyze(root);
        
        std::cout << "--- Semantic Analysis Done ---\n";
        
        // Вывод таблицы констант для проверки
        std::cout << "Constant Pool Dump:\n";
        const auto& entries = constPool.getEntries();
        for (size_t i = 1; i < entries.size(); ++i) {
            std::cout << "#" << i << " Tag: " << entries[i].tag << " ";
            if (entries[i].tag == CONSTANT_Utf8) std::cout << "Utf8: " << entries[i].stringValue;
            else if (entries[i].tag == CONSTANT_Integer) std::cout << "Int: " << entries[i].intValue;
            else if (entries[i].tag == CONSTANT_Float) std::cout << "Float: " << entries[i].floatValue;
            else if (entries[i].tag == CONSTANT_String) std::cout << "String Ref -> #" << entries[i].refIndex1;
            std::cout << "\n";
        }
        // -------------------------------------

        std::string dot_code = "digraph AST {\n" + root->toDotString() + "}\n";
        std::ofstream dot_file("output/ast.dot");

        if(dot_file.is_open()) {
            dot_file << dot_code;
            dot_file.close();

            std::cout << "\nAST created successfully in ast.dot";
            std::cout << "\nAttempting to run render.bat...\n";
            int system_result = std::system("render.bat");

            if(system_result != 0) {
                std::cerr << "Warning: render.bat failed to execute (Error code: " << system_result << ").\n";
            }
        }
        else {
            std::cerr << "Error: cannot create output/ast.dot\n";
        }
    }
    else {
        if(parse_result == 0) {
            std::cerr << "Warning: Parsing succeeded, but AST root was not created.\n";
        }
        else {
            std::cerr << "Error: AST root was not created due to parsing failure.\n";
        }
    }

    if(argc > 1 && yyin != stdin) {
        fclose(yyin);
    }

    if(parse_result != 0) {
        std::cerr << "Parsing ended with error\n";
        return(1);
    }

    return(0);
}