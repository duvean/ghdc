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

    if(yyparse() != 0) return(1);
    if (argc > 1) fclose(yyin);

    if (root) {
        std::string dot_code = root->toDotString();
        std::ofstream dot_file("ast.dot");
        if (dot_file.is_open()) {
            dot_file << dot_code;
            dot_file.close();
            std::cout << "\nAST generated in ast.dot\n";
            std::cout << "   To visualuze use: dot -Tpng ast.dot -o ast.png\n";
        } else {
            std::cerr << "Error: cannot create ast.dot\n";
            return(1);
        }
    } else {
        std::cerr << "Error: Root node wasn't created\n";
        return(1);
    }

    if (argc > 1) fclose(yyin);
    return(0);
}