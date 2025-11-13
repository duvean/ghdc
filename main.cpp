#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.tab.h"

extern FILE* yyin;
extern int yyparse();
extern int yylex();
extern ProgramNode* root;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if(!file) {
            fprintf(stderr, "Error opening file %s\n", argv[1]);
            return(1);
        }
    } 
    else yyin = stdin;

    int parse_result = yyparse();

    if (root) {
        std::string dot_code = root->toDotString();
        std::ofstream dot_file("ast.dot");
        if (dot_file.is_open()) {
            dot_file << dot_code;
            dot_file.close();
            
            std::cout << "\nAST created successfuly in ast.dot\n";
            std::cout << "   To visualize use: dot -Tpng ast.dot -o ast.png\n";
        } else {
            std::cerr << "Error: cannot create ast.dot\n";
        }
    } else {
        std::cerr << "Error: AST root was not created, no dot generated\n";
    }

    if (parse_result != 0) {
        std::cerr << "Parsing ended with error\n";
        return(1);
    }
    if (argc > 1) fclose(yyin);
    return(0);
}