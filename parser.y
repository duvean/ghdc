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
    DeclListNode* decl_list;
    ExprNode* expr;
    char* str;
    int intVal;
    double floatVal;
    char* strVal;
}

%type <program> program
%type <decl> decl func_signature func_definition let_block data_decl binding
%type <decl_list> decl_list decl_list_opt binding_list binding_list_opt
%type <decl_list> opt_where decl_block constr_list param_list
%type <expr> expr basic_expr type_expr app_expr pow_expr mult_expr add_expr comp_expr log_and_expr log_or_expr cons_expr compose_expr
%type <expr> expr_list expr_list_opt expr_list_tail
%type <expr> case_branch_list_opt case_branch_list case_branch
%type <expr> do_block do_stmt_list do_stmt
%type <expr> pattern pattern_list atomic_pattern constructor_app tuple_content

/* ==== Терминалы ==== */
%token KW_TRUE KW_FALSE
%token <str> ID ID_CAP
%token <str> DEC_LITERAL HEX_LITERAL OCT_LITERAL FLOAT
%token <strVal> CHAR_LITERAL STRING_LITERAL
%token KW_LET KW_WHERE KW_DO KW_OF KW_IN KW_CASE KW_IF KW_THEN KW_ELSE
%token KW_DATA KW_TYPE KW_RETURN
%token RIGHT_ARROW LEFT_ARROW FAT_ARROW DOUBLE_COLON
%token EQUALS PIPE COLON COMMA SEMICOLON BACKSLASH
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE
%token PLUS MINUS ASTERISK SLASH CARET DOUBLE_ASTERISK DOUBLE_CARET
%token LESS GREATER LESS_EQUAL GREATER_EQUAL DOUBLE_EQUALS NOT_EQUALS
%token DOUBLE_AMPERSAND DOUBLE_PIPE DOLLAR DOT
%token SINGLELINE_COMMENT DOUBLE_PLUS DOUBLE_DOT DOUBLE_BANG DOT_AMPERSAND_DOT DOT_PIPE_DOT DOUBLE_GREATER_EQUAL DOUBLE_GREATER EQUAL_DOUBLE_LESS AT_SIGN TILDE BANG PERCENT KW_DEFAULT KW_CLASS KW_INSTANCE KW_DERIVING KW_IMPORT KW_MODULE KW_FOREIGN KW_INFIX KW_INFIXR KW_AS KW_HIDING KW_QUALIFIED KW_NEWTYPE KW_EXPORT KW_CCALL KW_PRINT KW_GETLINE KW_INT KW_INTEGER KW_CHAR KW_STRING KW_FLOAT KW_BOOL BACKTICK KW_INFIXL AMPERSAND OPERATOR LBRACE RBRACE

/* ==== Приоритеты ==== */
%right KW_IN             /* in (для let...in). Имеет самый низкий приоритет. */
%nonassoc KW_IF KW_THEN KW_ELSE /* if/then/else. Не ассоциируются, низкий приоритет. */
%nonassoc KW_RETURN      /* return (если используется как оператор) */
%right RIGHT_ARROW       /* -> (для типов функций, лямбд) */
%right LEFT_ARROW        /* <- (для монадных выражений, not in this grammar) */
%right FAT_ARROW         /* => (для ограничений классов типов) */
%right EQUALS            /* = (для определений, not an expression operator) */
%right COLON             /* : (для составления списков: 1:[], list:tail) */
%right DOUBLE_COLON      /* :: (для аннотаций типов) */
%nonassoc DOUBLE_EQUALS NOT_EQUALS LESS GREATER LESS_EQUAL GREATER_EQUAL
%left DOUBLE_AMPERSAND   /* && (Логическое И) */
%left DOUBLE_PIPE        /* || (Логическое ИЛИ) */
%left PLUS MINUS         /* + - (Сложение/Вычитание) */
%left ASTERISK SLASH     /* * / (Умножение/Деление) */
%left DOUBLE_BANG        /* !! (Доступ к элементу списка) - Часто высокий, но тут ставим рядом с * / */
%right CARET DOUBLE_ASTERISK DOUBLE_CARET /* ^ ** ^^ (Степень) */
%right DOLLAR             /* $ (Оператор применения с низким приоритетом) */
%left DOT                /* . (Оператор композиции) */
/* Маркер для завершения do-оператора: самая низкая приоритетность.
   Правило do_stmt: expr будет иметь этот приоритет, поэтому при появлении
   оператора с более высоким приоритетом бизон предпочтёт шифт (продолжить expr).
*/
%nonassoc DO_STMT_TERM
%left APPLY_PREC

%start program

%%

program: decl_list { root = ProgramNode::create($1); $$ = root; }

decl_list:
      decl_list decl { $$ = DeclListNode::addDecl($1, $2); }
    | decl           { $$ = DeclListNode::create($1); }
    ;

decl_list_opt:
      decl_list  { $$ = $1; }
    | /* void */ { $$ = nullptr; }
    ;

decl_block:
      LEFT_BRACE decl_list_opt RIGHT_BRACE { $$ = $2; }
    ;

opt_where:
      KW_WHERE decl_block
    | /* void */
    ;

decl:
      func_signature
    | func_definition
    | let_block
    | data_decl
    | binding
    ;

/* --- Объявление переменной --- */
let_block:
      KW_LET LEFT_BRACE binding_list_opt RIGHT_BRACE { $$ = DeclNode::createLetBlock($3); }
    ;

binding_list:
      binding_list binding { $$ = DeclListNode::addDecl($1, $2); }
    | binding              { $$ = DeclListNode::create($1); }
    ;

binding_list_opt:
      binding_list { $$ = $1; }
    | /* void */   { $$ = nullptr; }
    ;

binding:
      ID EQUALS expr semicolon_opt { $$ = DeclNode::createVarDecl($1, $3); }
      // если будут беды со свёрткой decl->decl_list поменять на SEMICOLON
    ;

/* --- Объявление функции --- */
func_definition:
      ID param_list EQUALS expr opt_where { $$ = DeclNode::createFuncDef($1, $2, $4, $5); }
    ;

func_signature:
      ID DOUBLE_COLON type_expr SEMICOLON { $$ = DeclNode::createFuncSignature($1, $3); }
    ;

type_expr:
      KW_INT                  { $$ = ExprNode::createPrimitiveType("Int"); }
    | KW_FLOAT                { $$ = ExprNode::createPrimitiveType("Float"); }
    | KW_CHAR                 { $$ = ExprNode::createPrimitiveType("Char"); }
    | KW_STRING               { $$ = ExprNode::createPrimitiveType("String"); }
    | KW_BOOL                 { $$ = ExprNode::createPrimitiveType("Bool"); }
    | ID_CAP                  { $$ = ExprNode::createTypeConstructor($1); } // Maybe, List
    | ID                      { $$ = ExprNode::createTypeVar($1); } // a, b (переменная типа)
    | type_expr RIGHT_ARROW type_expr  { $$ = ExprNode::createFunctionType($1, $3); } // Функциональный тип (->)
    | LEFT_PAREN type_expr RIGHT_PAREN { $$ = $2; } // Группировка
    ;

/* --- Определение типа данных --- */
data_decl:
      KW_DATA ID_CAP EQUALS constr_list { $$ = DeclNode::createDataDecl($2, $4); }
    ;

constr_list:
      constr_list PIPE ID_CAP { $$ = DeclListNode::addConstructorToList($1, $3); }
    | ID_CAP                  { $$ = DeclListNode::createConstructorList($1); }
    ;












/* --- Выражения --- */

basic_expr:
	  DEC_LITERAL    { $$ = ExprNode::createLiteral($1); }
    | HEX_LITERAL    { $$ = ExprNode::createLiteral($1); }
    | OCT_LITERAL    { $$ = ExprNode::createLiteral($1); }
    | FLOAT          { $$ = ExprNode::createLiteral($1); }
    | KW_TRUE        { $$ = ExprNode::createLiteral("True"); }
    | KW_FALSE       { $$ = ExprNode::createLiteral("False"); }
	| CHAR_LITERAL   { $$ = ExprNode::createLiteral($1); }
	| STRING_LITERAL { $$ = ExprNode::createLiteral($1); }
    | ID             { $$ = ExprNode::createVarRef($1); }
    | ID_CAP         { $$ = ExprNode::createVarRef($1); }
    | LEFT_BRACKET expr_list_opt RIGHT_BRACKET { $$ = ExprNode::createArrayExpr($2); }
    | LEFT_PAREN expr_list_opt RIGHT_PAREN     { $$ = ExprNode::createTupleExpr($2); }
    | LEFT_PAREN expr RIGHT_PAREN              { $$ = $2; }

app_expr: basic_expr
        | app_expr basic_expr %prec APPLY_PREC { $$ = ExprNode::createFuncCall($1, $2); }
;

/*
 * Уровень 10: Операторы степени (Право-ассоциативны)
 * Использует app_expr в правой части
 */
pow_expr: app_expr
        | app_expr CARET pow_expr           { $$ = ExprNode::createBinaryExpr("^", $1, $3); }
        | app_expr DOUBLE_ASTERISK pow_expr { $$ = ExprNode::createBinaryExpr("**", $1, $3); }
        | app_expr DOUBLE_CARET pow_expr    { $$ = ExprNode::createBinaryExpr("^^", $1, $3); }
;

/*
 * Уровень 9: Мультипликативные (Лево-ассоциативны)
 * Использует pow_expr в правой части
 */
mult_expr: pow_expr
         | mult_expr ASTERISK pow_expr      { $$ = ExprNode::createBinaryExpr("*", $1, $3); }
         | mult_expr SLASH pow_expr         { $$ = ExprNode::createBinaryExpr("/", $1, $3); }
         | mult_expr DOUBLE_BANG pow_expr   { $$ = ExprNode::createBinaryExpr("!!", $1, $3); }
;

/*
 * Уровень 8: Аддитивные (Лево-ассоциативны)
 * Использует mult_expr в правой части
 */
add_expr: mult_expr
        | add_expr PLUS mult_expr           { $$ = ExprNode::createBinaryExpr("+", $1, $3); }
        | add_expr MINUS mult_expr          { $$ = ExprNode::createBinaryExpr("-", $1, $3); }
;

/*
 * Уровень 6: Сравнение (Не-ассоциативны)
 * Использует add_expr в правой части
 */
comp_expr: add_expr
         | add_expr DOUBLE_EQUALS add_expr  { $$ = ExprNode::createBinaryExpr("==", $1, $3); }
         | add_expr NOT_EQUALS add_expr     { $$ = ExprNode::createBinaryExpr("!=", $1, $3); }
         | add_expr LESS add_expr           { $$ = ExprNode::createBinaryExpr("<", $1, $3); }
         | add_expr GREATER add_expr        { $$ = ExprNode::createBinaryExpr(">", $1, $3); }
         | add_expr LESS_EQUAL add_expr     { $$ = ExprNode::createBinaryExpr("<=", $1, $3); }
         | add_expr GREATER_EQUAL add_expr  { $$ = ExprNode::createBinaryExpr(">=", $1, $3); }
;

/*
 * Уровень 7 (Логическое И, ниже 6)
 * Примечание: В Haskell && (Приоритет 3) и || (Приоритет 2)
 */
log_and_expr: comp_expr
            | log_and_expr DOUBLE_AMPERSAND comp_expr { $$ = ExprNode::createBinaryExpr("&&", $1, $3); }
;

log_or_expr: log_and_expr
           | log_and_expr DOUBLE_PIPE log_or_expr   { $$ = ExprNode::createBinaryExpr("||", $1, $3); }
;

/*
 * Уровень 5: Cons-оператор ':' (Право-ассоциативен)
 */
cons_expr: log_or_expr
         | log_or_expr COLON cons_expr      { $$ = ExprNode::createBinaryExpr(":", $1, $3); }
;

/*
 * Уровень 11: Аннотации, Композиция и Применение ($)
 */
compose_expr: cons_expr
            | cons_expr DOUBLE_COLON type_expr %prec DOUBLE_COLON { $$ = ExprNode::createTypeAnnotation($1, $3); }
            | compose_expr DOT cons_expr %prec DOT               { $$ = ExprNode::createBinaryExpr(".", $1, $3); }
            | cons_expr DOLLAR compose_expr %prec DOLLAR         { $$ = ExprNode::createBinaryExpr("$", $1, $3); }
;

/*
 * Итоговое выражение (expr) включает все, что ниже (лямбды, if, let, do)
 */
expr: compose_expr
    /* лямбда */
    | BACKSLASH expr RIGHT_ARROW expr %prec RIGHT_ARROW { $$ = ExprNode::createLambda($2, $4); }

    /* if / case */
    | KW_IF expr KW_THEN expr KW_ELSE expr %prec KW_IF { $$ = ExprNode::createIfExpr($2, $4, $6); }
    | KW_CASE expr KW_OF LEFT_BRACE case_branch_list_opt RIGHT_BRACE { $$ = ExprNode::createCaseExpr($2, $5); }

    /* let ... in ... */
    | let_block KW_IN expr %prec KW_IN { $$ = ExprNode::createLetInExpr($1, $3); }

    /* do-нотация */
    | KW_DO do_block { $$ = ExprNode::createDoExpr($2); }

    /* return */
    | KW_RETURN expr %prec KW_RETURN { $$ = ExprNode::createReturnExpr($2); }
;

expr_list_opt:
      expr_list { $$ = $1; }
    | /* void */     { $$ = nullptr; }
    ;

expr_list:
      cons_expr COMMA expr_list_tail {
          ExprNode* list = ExprNode::createExprList($1);
          $$ = ExprNode::addExprToList(list, $3);
      }
    ;

expr_list_tail:
      cons_expr { $$ = ExprNode::createExprList($1); }
    | cons_expr COMMA expr_list_tail {
          ExprNode* list = ExprNode::createExprList($1);
          $$ = ExprNode::addExprToList(list, $3);
      }
    ;
	




/* --- Сопоставление с образцом (Patterns) --- */
atomic_pattern:
	  ID							                  { $$ = ExprNode::createVarPattern($1); } // Переменная или Wildcard (если ID = "_")
	| DEC_LITERAL				                      { $$ = ExprNode::createLiteralPattern($1); }
	| KW_TRUE					                      { $$ = ExprNode::createLiteralPattern("True"); }
    | KW_FALSE					                      { $$ = ExprNode::createLiteralPattern("False"); }
	| LEFT_PAREN RIGHT_PAREN                  		  { $$ = nullptr; } // Пустой кортеж
	| LEFT_PAREN tuple_content RIGHT_PAREN		      { $$ = ExprNode::createTuplePattern($2); } // Образец кортежа
	| LEFT_BRACKET RIGHT_BRACKET	                  { $$ = nullptr; } // Пустой список
	| LEFT_BRACKET pattern_list RIGHT_BRACKET	      { $$ = ExprNode::createListPattern($2); } // Образец списка
	| LEFT_PAREN pattern RIGHT_PAREN	              { $$ = $2; } // Группировка
	;
constructor_app:
	  ID_CAP											{ $$ = ExprNode::createConstructorPattern($1, nullptr); }
	| constructor_app atomic_pattern %prec APPLY_PREC	{ $$ = ExprNode::addArgumentToConstructor($1, $2); }
	;
pattern:
	  constructor_app
	| atomic_pattern
	| pattern COLON pattern		{ $$ = ExprNode::createConsPattern($1, $3); }
	;
tuple_content:
    pattern COMMA pattern_list { $$ = ExprNode::addPatternToList($1, $3); }
pattern_list:
	pattern COMMA pattern_list { $$ = ExprNode::addPatternToList($1, $3); }
	| pattern                  { $$ = ExprNode::createPatternList($1); }
	;


/* --- Список параметров функции --- */
param_list:
      param_list pattern { $$ = DeclListNode::addParamToList($1, $2); }
    | pattern            { $$ = DeclListNode::createParamList($1); }
    ;

/* --- Ветки case --- */
case_branch_list_opt:
      case_branch_list { $$ = $1; }
    | /* void */       { $$ = nullptr; }
    ;

case_branch_list:
      case_branch SEMICOLON case_branch_list { $$ = ExprNode::addCaseBranchToList($1, $3); }
    | case_branch SEMICOLON { $$ = ExprNode::createCaseBranchList($1); }
    ;

case_branch:
      pattern RIGHT_ARROW expr { $$ = ExprNode::createCaseBranch($1, $3); }
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

semicolon_opt:
      SEMICOLON
    | /* void */
    ;

%%

void yyerror(const char *s) {
    extern int yylineno;
    extern char *yytext;
    fprintf(stderr, "Parser error: %s at line %d, near token '%s'\n", s, yylineno, yytext);
}
