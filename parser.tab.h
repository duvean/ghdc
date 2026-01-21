
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
     KW_TRUE = 258,
     KW_FALSE = 259,
     ID = 260,
     ID_CAP = 261,
     DEC_LITERAL = 262,
     HEX_LITERAL = 263,
     OCT_LITERAL = 264,
     FLOAT = 265,
     CHAR_LITERAL = 266,
     STRING_LITERAL = 267,
     KW_LET = 268,
     KW_WHERE = 269,
     KW_DO = 270,
     KW_OF = 271,
     KW_IN = 272,
     KW_CASE = 273,
     KW_IF = 274,
     KW_THEN = 275,
     KW_ELSE = 276,
     KW_DATA = 277,
     KW_RETURN = 278,
     PIPE = 279,
     COMMA = 280,
     SEMICOLON = 281,
     BACKSLASH = 282,
     LEFT_PAREN = 283,
     RIGHT_PAREN = 284,
     LEFT_BRACKET = 285,
     RIGHT_BRACKET = 286,
     LEFT_BRACE = 287,
     RIGHT_BRACE = 288,
     KW_INT = 289,
     KW_BOOL = 290,
     KW_CHAR = 291,
     KW_STRING = 292,
     KW_FLOAT = 293,
     EQUALS = 294,
     LEFT_ARROW = 295,
     RIGHT_ARROW = 296,
     FAT_ARROW = 297,
     DO_STMT_TERM = 298,
     DOLLAR = 299,
     DOUBLE_COLON = 300,
     DOUBLE_PIPE = 301,
     DOUBLE_AMPERSAND = 302,
     GREATER_EQUAL = 303,
     LESS_EQUAL = 304,
     GREATER = 305,
     LESS = 306,
     NOT_EQUALS = 307,
     DOUBLE_EQUALS = 308,
     COLON = 309,
     MINUS = 310,
     PLUS = 311,
     SLASH = 312,
     ASTERISK = 313,
     DOUBLE_CARET = 314,
     DOUBLE_ASTERISK = 315,
     CARET = 316,
     DOT = 317,
     DOUBLE_BANG = 318,
     APPLY_PREC = 319
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 21 "parser.y"

    ProgramNode* program;
    DeclNode* decl;
    DeclListNode* decl_list;
    ExprNode* expr;
    char* str;
    int intVal;
    double floatVal;
    char* strVal;



/* Line 1676 of yacc.c  */
#line 138 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


