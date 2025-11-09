%code requires { #include "ast.h" }
%debug
%{

#include "ast.h"
#include "token_queue.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

ProgramNode* root = nullptr;
extern int yylex(void);
int get_next_token(void);
#define yylex get_next_token
void yyerror(const char *s);
%}

%union {
    ProgramNode* program;
    DeclNode* decl;
    ExprNode* expr;
    char* str;
    int intVal;
    double floatVal;
    char* strVal;
}

%type <program> program
%type <decl> decl func_signature func_definition var_decl data_decl decl_list decl_list_opt
%type <decl> opt_where decl_block constr_list var_decl_block param_list
%type <expr> expr expr_list expr_list_opt expr_list_tail
%type <expr> type_expr
%type <expr> case_branch_list_opt case_branch_list case_branch
%type <expr> do_block do_stmt_list do_stmt


/* ==== Терминалы ==== */
%token KW_LET KW_WHERE KW_DO KW_OF KW_IN KW_CASE KW_IF KW_THEN KW_ELSE
%token KW_DATA KW_TYPE KW_RETURN
%token KW_TRUE KW_FALSE
%token <str> ID ID_CAP
%token <str> DEC_LITERAL HEX_LITERAL OCT_LITERAL FLOAT
%token RIGHT_ARROW LEFT_ARROW FAT_ARROW DOUBLE_COLON
%token EQUALS PIPE COLON COMMA SEMICOLON BACKSLASH
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE
%token PLUS MINUS ASTERISK SLASH CARET DOUBLE_ASTERISK DOUBLE_CARET
%token LESS GREATER LESS_EQUAL GREATER_EQUAL DOUBLE_EQUALS NOT_EQUALS
%token DOUBLE_AMPERSAND DOUBLE_PIPE DOLLAR DOT
%token SINGLELINE_COMMENT DOUBLE_PLUS DOUBLE_DOT DOUBLE_BANG DOT_AMPERSAND_DOT DOT_PIPE_DOT DOUBLE_GREATER_EQUAL DOUBLE_GREATER EQUAL_DOUBLE_LESS AT_SIGN TILDE BANG PERCENT KW_DEFAULT KW_CLASS KW_INSTANCE KW_DERIVING KW_IMPORT KW_MODULE KW_FOREIGN KW_INFIX KW_INFIXR KW_AS KW_HIDING KW_QUALIFIED KW_NEWTYPE KW_EXPORT KW_CCALL KW_PRINT KW_GETLINE KW_INT KW_INTEGER KW_CHAR KW_STRING KW_FLOAT KW_BOOL BACKTICK KW_INFIXL AMPERSAND OPERATOR CHAR_LITERAL STRING_LITERAL LBRACE RBRACE

/* ==== Приоритеты ==== */
%left DOT
%right DOLLAR
%left DOUBLE_PIPE
%left DOUBLE_AMPERSAND
%nonassoc DOUBLE_EQUALS NOT_EQUALS LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left ASTERISK SLASH
%right CARET DOUBLE_ASTERISK DOUBLE_CARET
%right COLON
%right RIGHT_ARROW LEFT_ARROW FAT_ARROW EQUALS DOUBLE_COLON
%nonassoc KW_IF KW_THEN KW_ELSE
%nonassoc KW_RETURN
%nonassoc KW_LET
/* Маркер для завершения do-оператора: самая низкая приоритетность.
   Правило do_stmt: expr будет иметь этот приоритет, поэтому при появлении
   оператора с более высоким приоритетом бизон предпочтёт шифт (продолжить expr).
*/
%nonassoc DO_STMT_TERM

%start program

%%

program: decl_list { root = ProgramNode::create($1->block); $$ = root; }

decl_list:
      decl_list decl { $$ = DeclNode::addDeclToList($1, $2); }
    | decl           { $$ = DeclNode::createDeclList($1); }
    ;

decl_list_opt:
      decl_list  { $$ = $1; }
    | /* void */ { $$ = nullptr; }
    ;

decl_block:
      LEFT_BRACE decl_list_opt RIGHT_BRACE
    ;

opt_where:
      KW_WHERE decl_block
    | /* void */
    ;

decl:
      func_signature
    | func_definition
    | var_decl
    | var_decl_block
    | data_decl
    ;

/* --- Объявление переменной --- */
var_decl:
      KW_LET var_decl_block                       { $$ = $2; } 
    | KW_LET LEFT_BRACE decl_list_opt RIGHT_BRACE { $$ = $3; }
    ;

var_decl_block:
      ID EQUALS expr SEMICOLON { $$ = DeclNode::createVarDecl($1, $3); }
    ;

/* --- Объявление функции --- */
func_definition:
      ID param_list EQUALS expr opt_where { $$ = DeclNode::createFuncDef($1, $2, $4, $5); }
    | ID EQUALS expr opt_where            { $$ = DeclNode::createFuncDef($1, nullptr, $3, $4); } 
    ;

func_signature:
      ID DOUBLE_COLON type_expr SEMICOLON { $$ = DeclNode::createFuncSignature($1, $3); }
    ;

type_expr:
    | KW_INT
    | KW_FLOAT
    | KW_CHAR
    | KW_STRING
    | KW_BOOL
    | ID_CAP               // Конструктор типа (напр., Int, Maybe)
    | ID                   // Переменная типа (напр., a, b)
    | type_expr RIGHT_ARROW type_expr // Функциональный тип (напр., Int -> Int)
    | LEFT_PAREN type_expr RIGHT_PAREN
    ;

/* --- Определение типа данных --- */
data_decl:
      KW_DATA ID_CAP EQUALS constr_list { $$ = DeclNode::createDataDecl($2, $4); }
    ;

constr_list:
      constr_list PIPE ID_CAP
    | ID_CAP
    ;












/* --- Выражения --- */
expr:
      DEC_LITERAL { $$ = ExprNode::createLiteral($1); }
    | HEX_LITERAL { $$ = ExprNode::createLiteral($1); }
    | OCT_LITERAL { $$ = ExprNode::createLiteral($1); }
    | FLOAT       { $$ = ExprNode::createLiteral($1); }
    | KW_TRUE     { $$ = ExprNode::createLiteral("true"); }
    | KW_FALSE    { $$ = ExprNode::createLiteral("false"); }
    | ID          { $$ = ExprNode::createVarRef($1); }
    | ID_CAP      { $$ = ExprNode::createVarRef($1); }
    | LEFT_BRACKET expr_list_opt RIGHT_BRACKET { $$ = ExprNode::createArrayExpr($2); }
    | LEFT_PAREN expr_list_opt RIGHT_PAREN     { $$ = ExprNode::createTupleExpr($2); }
    | LEFT_PAREN expr RIGHT_PAREN              { $$ = $2; }

    /* арифметика */
    | expr PLUS expr { $$ = ExprNode::createBinaryExpr("+", $1, $3); }
    | expr MINUS expr
    | expr ASTERISK expr
    | expr SLASH expr
    | expr CARET expr
    | expr DOUBLE_ASTERISK expr
    | expr DOUBLE_CARET expr

    /* логика и сравнение */
    | expr DOUBLE_EQUALS expr
    | expr NOT_EQUALS expr
    | expr LESS expr
    | expr GREATER expr
    | expr LESS_EQUAL expr
    | expr GREATER_EQUAL expr
    | expr DOUBLE_AMPERSAND expr
    | expr DOUBLE_PIPE expr

    /* функциональные операторы */
    | expr COLON expr
    | expr DOLLAR expr
    | expr DOT expr
    | expr DOUBLE_COLON type_expr  /* аннотация типа */

    /* лямбда */
    | BACKSLASH expr RIGHT_ARROW expr { $$ = ExprNode::createLambda($2, $4); }

    /* if / case */
    | KW_IF expr KW_THEN expr KW_ELSE expr %prec KW_IF { $$ = ExprNode::createIfExpr($2, $4, $6); }
    | KW_CASE expr KW_OF LEFT_BRACE case_branch_list_opt RIGHT_BRACE { $$ = ExprNode::createCaseExpr($2, $5); }

    /* let ... in ... */
    | KW_LET decl_list KW_IN expr %prec KW_LET { $$ = ExprNode::createLetInExpr($2, $4); }

    /* do-нотация */
    | KW_DO do_block { $$ = ExprNode::createDoExpr($2); }

    /* return */
    | KW_RETURN expr %prec KW_RETURN { $$ = ExprNode::createReturnExpr($2); }
    ;

expr_list_opt:
      expr_list  { $$ = $1; }
    | /* void */ { $$ = nullptr; }
    ;

expr_list:
      expr COMMA expr_list_tail {
            ExprNode* list = ExprNode::createExprList($1);
            $$ = ExprNode::addExprToList(list, $3);
        }
    ;
    
expr_list_tail:
      expr { $$ = ExprNode::createExprList($1); }
    | expr COMMA expr_list_tail { 
          ExprNode* list = ExprNode::createExprList($1);
          $$ = ExprNode::addExprToList(list, $3); 
      }
    ;

/* --- Список параметров функции --- */
param_list:
      param_list ID { $$ = DeclNode::addParamToList($1, $2); }
    | ID            { $$ = DeclNode::createParamList($1); }
    ;

/* --- Ветки case --- */
case_branch_list_opt:
      case_branch_list
    | /* void (case x of {}) */
    ;

case_branch_list:
      case_branch
    | case_branch_list SEMICOLON case_branch
    ;

case_branch:
      expr RIGHT_ARROW expr
    ;

/* --- do-блок --- */
do_block:
      LEFT_BRACE do_stmt_list RIGHT_BRACE
    ;

do_stmt_list:
      do_stmt_list SEMICOLON do_stmt
    | do_stmt
    ;

do_stmt:
      expr LEFT_ARROW expr       /* x <- action  — однозначно */
    | expr %prec DO_STMT_TERM    /* действие-выражение: reduce только если lookahead имеет НИЖ. приоритет */
    ;

%%

void yyerror(const char *s) {
    extern int yylineno;
    extern char *yytext;
    fprintf(stderr, "Parser error: %s at line %d, near token '%s'\n", s, yylineno, yytext);
}
