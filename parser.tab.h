
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 1 "parser.y"
 #include "ast.h" 


/* Line 1676 of yacc.c  */
#line 44 "parser.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KW_LET = 258,
     KW_WHERE = 259,
     KW_DO = 260,
     KW_OF = 261,
     KW_IN = 262,
     KW_CASE = 263,
     KW_IF = 264,
     KW_THEN = 265,
     KW_ELSE = 266,
     KW_DATA = 267,
     KW_TYPE = 268,
     KW_RETURN = 269,
     KW_TRUE = 270,
     KW_FALSE = 271,
     ID = 272,
     ID_CAP = 273,
     DEC_LITERAL = 274,
     HEX_LITERAL = 275,
     OCT_LITERAL = 276,
     FLOAT = 277,
     RIGHT_ARROW = 278,
     LEFT_ARROW = 279,
     FAT_ARROW = 280,
     DOUBLE_COLON = 281,
     EQUALS = 282,
     PIPE = 283,
     COLON = 284,
     COMMA = 285,
     SEMICOLON = 286,
     BACKSLASH = 287,
     LEFT_PAREN = 288,
     RIGHT_PAREN = 289,
     LEFT_BRACKET = 290,
     RIGHT_BRACKET = 291,
     LEFT_BRACE = 292,
     RIGHT_BRACE = 293,
     PLUS = 294,
     MINUS = 295,
     ASTERISK = 296,
     SLASH = 297,
     CARET = 298,
     DOUBLE_ASTERISK = 299,
     DOUBLE_CARET = 300,
     LESS = 301,
     GREATER = 302,
     LESS_EQUAL = 303,
     GREATER_EQUAL = 304,
     DOUBLE_EQUALS = 305,
     NOT_EQUALS = 306,
     DOUBLE_AMPERSAND = 307,
     DOUBLE_PIPE = 308,
     DOLLAR = 309,
     DOT = 310,
     SINGLELINE_COMMENT = 311,
     DOUBLE_PLUS = 312,
     DOUBLE_DOT = 313,
     DOUBLE_BANG = 314,
     DOT_AMPERSAND_DOT = 315,
     DOT_PIPE_DOT = 316,
     DOUBLE_GREATER_EQUAL = 317,
     DOUBLE_GREATER = 318,
     EQUAL_DOUBLE_LESS = 319,
     AT_SIGN = 320,
     TILDE = 321,
     BANG = 322,
     PERCENT = 323,
     KW_DEFAULT = 324,
     KW_CLASS = 325,
     KW_INSTANCE = 326,
     KW_DERIVING = 327,
     KW_IMPORT = 328,
     KW_MODULE = 329,
     KW_FOREIGN = 330,
     KW_INFIX = 331,
     KW_INFIXR = 332,
     KW_AS = 333,
     KW_HIDING = 334,
     KW_QUALIFIED = 335,
     KW_NEWTYPE = 336,
     KW_EXPORT = 337,
     KW_CCALL = 338,
     KW_PRINT = 339,
     KW_GETLINE = 340,
     KW_INT = 341,
     KW_INTEGER = 342,
     KW_CHAR = 343,
     KW_STRING = 344,
     KW_FLOAT = 345,
     KW_BOOL = 346,
     BACKTICK = 347,
     KW_INFIXL = 348,
     AMPERSAND = 349,
     OPERATOR = 350,
     CHAR_LITERAL = 351,
     STRING_LITERAL = 352,
     LBRACE = 353,
     RBRACE = 354,
     DO_STMT_TERM = 355
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 20 "parser.y"

    ProgramNode* program;
    DeclNode* decl;
    ExprNode* expr;
    char* str;
    int intVal;
    double floatVal;
    char* strVal;



/* Line 1676 of yacc.c  */
#line 173 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


