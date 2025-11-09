#include <iostream>
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
    else { 
        yyin = stdin;
    }
    if(yyparse() != 0) {
         fprintf(stdout, "Parsing error in main\n");
         return(1);
    }
    if (argc > 1) fclose(yyin);
    return(0);
}