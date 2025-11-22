
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 3 "parser.y"


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


/* Line 189 of yacc.c  */
#line 91 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 1 "parser.y"
 #include "ast.h" 


/* Line 209 of yacc.c  */
#line 119 "parser.tab.c"

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
     KW_TYPE = 278,
     KW_RETURN = 279,
     RIGHT_ARROW = 280,
     LEFT_ARROW = 281,
     FAT_ARROW = 282,
     DOUBLE_COLON = 283,
     EQUALS = 284,
     PIPE = 285,
     COLON = 286,
     COMMA = 287,
     SEMICOLON = 288,
     BACKSLASH = 289,
     LEFT_PAREN = 290,
     RIGHT_PAREN = 291,
     LEFT_BRACKET = 292,
     RIGHT_BRACKET = 293,
     LEFT_BRACE = 294,
     RIGHT_BRACE = 295,
     PLUS = 296,
     MINUS = 297,
     ASTERISK = 298,
     SLASH = 299,
     CARET = 300,
     DOUBLE_ASTERISK = 301,
     DOUBLE_CARET = 302,
     LESS = 303,
     GREATER = 304,
     LESS_EQUAL = 305,
     GREATER_EQUAL = 306,
     DOUBLE_EQUALS = 307,
     NOT_EQUALS = 308,
     DOUBLE_AMPERSAND = 309,
     DOUBLE_PIPE = 310,
     DOLLAR = 311,
     DOT = 312,
     SINGLELINE_COMMENT = 313,
     DOUBLE_PLUS = 314,
     DOUBLE_DOT = 315,
     DOUBLE_BANG = 316,
     DOT_AMPERSAND_DOT = 317,
     DOT_PIPE_DOT = 318,
     DOUBLE_GREATER_EQUAL = 319,
     DOUBLE_GREATER = 320,
     EQUAL_DOUBLE_LESS = 321,
     AT_SIGN = 322,
     TILDE = 323,
     BANG = 324,
     PERCENT = 325,
     KW_DEFAULT = 326,
     KW_CLASS = 327,
     KW_INSTANCE = 328,
     KW_DERIVING = 329,
     KW_IMPORT = 330,
     KW_MODULE = 331,
     KW_FOREIGN = 332,
     KW_INFIX = 333,
     KW_INFIXR = 334,
     KW_AS = 335,
     KW_HIDING = 336,
     KW_QUALIFIED = 337,
     KW_NEWTYPE = 338,
     KW_EXPORT = 339,
     KW_CCALL = 340,
     KW_PRINT = 341,
     KW_GETLINE = 342,
     KW_INT = 343,
     KW_INTEGER = 344,
     KW_CHAR = 345,
     KW_STRING = 346,
     KW_FLOAT = 347,
     KW_BOOL = 348,
     BACKTICK = 349,
     KW_INFIXL = 350,
     AMPERSAND = 351,
     OPERATOR = 352,
     LBRACE = 353,
     RBRACE = 354,
     DO_STMT_TERM = 355,
     APPLY_PREC = 356
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 20 "parser.y"

    ProgramNode* program;
    DeclNode* decl;
    DeclListNode* decl_list;
    ExprNode* expr;
    char* str;
    int intVal;
    double floatVal;
    char* strVal;



/* Line 214 of yacc.c  */
#line 250 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 262 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  27
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   262

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNRULES -- Number of states.  */
#define YYNSTATES  203

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   356

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    13,    17,    20,
      21,    23,    25,    27,    29,    31,    36,    39,    41,    43,
      44,    49,    55,    60,    62,    64,    66,    68,    70,    72,
      74,    78,    82,    86,    91,    95,    97,    99,   101,   103,
     105,   107,   109,   111,   113,   115,   117,   121,   125,   129,
     132,   135,   137,   140,   142,   146,   150,   154,   156,   160,
     164,   168,   170,   174,   178,   180,   184,   188,   192,   196,
     200,   204,   206,   210,   212,   216,   218,   222,   224,   228,
     232,   236,   238,   243,   250,   257,   261,   264,   267,   271,
     275,   277,   279,   281,   283,   285,   288,   292,   295,   299,
     303,   305,   308,   310,   312,   316,   320,   324,   326,   329,
     331,   333,   334,   337,   341,   345,   349,   353,   355,   359,
     361,   363
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     103,     0,    -1,   104,    -1,   104,   108,    -1,   108,    -1,
     104,    -1,    -1,    39,   105,    40,    -1,    14,   106,    -1,
      -1,   114,    -1,   113,    -1,   109,    -1,   116,    -1,   112,
      -1,    13,    39,   111,    40,    -1,   110,   112,    -1,   112,
      -1,   110,    -1,    -1,     5,    29,   128,   143,    -1,     5,
     136,    29,   128,   107,    -1,     5,    28,   115,    33,    -1,
      88,    -1,    92,    -1,    90,    -1,    91,    -1,    93,    -1,
       6,    -1,     5,    -1,   115,    25,   115,    -1,    35,   115,
      36,    -1,    37,   115,    38,    -1,    22,     6,    29,   117,
      -1,   117,    30,     6,    -1,     6,    -1,     7,    -1,     8,
      -1,     9,    -1,    10,    -1,     3,    -1,     4,    -1,    11,
      -1,    12,    -1,     5,    -1,     6,    -1,    35,   127,    36,
      -1,    35,   129,    36,    -1,    37,   130,    38,    -1,    35,
      36,    -1,    37,    38,    -1,   118,    -1,   119,   118,    -1,
     119,    -1,   119,    45,   120,    -1,   119,    46,   120,    -1,
     119,    47,   120,    -1,   120,    -1,   121,    43,   120,    -1,
     121,    44,   120,    -1,   121,    61,   120,    -1,   121,    -1,
     122,    41,   121,    -1,   122,    42,   121,    -1,   122,    -1,
     122,    52,   122,    -1,   122,    53,   122,    -1,   122,    48,
     122,    -1,   122,    49,   122,    -1,   122,    50,   122,    -1,
     122,    51,   122,    -1,   123,    -1,   124,    54,   123,    -1,
     124,    -1,   124,    55,   125,    -1,   125,    -1,   125,    31,
     126,    -1,   126,    -1,   127,    57,   126,    -1,   127,    56,
     127,    -1,   127,    28,   115,    -1,   127,    -1,    34,   128,
      25,   128,    -1,    19,   128,    20,   128,    21,   128,    -1,
      18,   128,    16,    39,   137,    40,    -1,   109,    17,   128,
      -1,    15,   140,    -1,    24,   128,    -1,   126,    32,   130,
      -1,   126,    32,   130,    -1,   126,    -1,     5,    -1,     7,
      -1,     3,    -1,     4,    -1,    35,    36,    -1,    35,   134,
      36,    -1,    37,    38,    -1,    37,   135,    38,    -1,    35,
     133,    36,    -1,     6,    -1,   132,   131,    -1,   132,    -1,
     131,    -1,   133,    31,   133,    -1,   133,    32,   135,    -1,
     133,    32,   135,    -1,   133,    -1,   136,   133,    -1,   133,
      -1,   138,    -1,    -1,   139,    33,    -1,   138,   139,    33,
      -1,   133,    25,   128,    -1,    39,   141,    40,    -1,   141,
      33,   142,    -1,   142,    -1,   128,    26,   128,    -1,   128,
      -1,    33,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   105,   105,   108,   109,   113,   114,   118,   122,   123,
     127,   128,   129,   130,   131,   136,   140,   141,   145,   146,
     150,   156,   160,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   178,   182,   183,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   217,   218,   225,   226,   227,   228,   235,   236,   237,
     238,   245,   246,   247,   254,   255,   256,   257,   258,   259,
     260,   267,   268,   271,   272,   278,   279,   285,   286,   287,
     288,   294,   296,   299,   300,   303,   306,   309,   313,   315,
     316,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     336,   337,   340,   341,   342,   345,   347,   348,   354,   355,
     360,   361,   365,   367,   372,   377,   381,   382,   386,   387,
     391,   392
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_TRUE", "KW_FALSE", "ID", "ID_CAP",
  "DEC_LITERAL", "HEX_LITERAL", "OCT_LITERAL", "FLOAT", "CHAR_LITERAL",
  "STRING_LITERAL", "KW_LET", "KW_WHERE", "KW_DO", "KW_OF", "KW_IN",
  "KW_CASE", "KW_IF", "KW_THEN", "KW_ELSE", "KW_DATA", "KW_TYPE",
  "KW_RETURN", "RIGHT_ARROW", "LEFT_ARROW", "FAT_ARROW", "DOUBLE_COLON",
  "EQUALS", "PIPE", "COLON", "COMMA", "SEMICOLON", "BACKSLASH",
  "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET",
  "LEFT_BRACE", "RIGHT_BRACE", "PLUS", "MINUS", "ASTERISK", "SLASH",
  "CARET", "DOUBLE_ASTERISK", "DOUBLE_CARET", "LESS", "GREATER",
  "LESS_EQUAL", "GREATER_EQUAL", "DOUBLE_EQUALS", "NOT_EQUALS",
  "DOUBLE_AMPERSAND", "DOUBLE_PIPE", "DOLLAR", "DOT", "SINGLELINE_COMMENT",
  "DOUBLE_PLUS", "DOUBLE_DOT", "DOUBLE_BANG", "DOT_AMPERSAND_DOT",
  "DOT_PIPE_DOT", "DOUBLE_GREATER_EQUAL", "DOUBLE_GREATER",
  "EQUAL_DOUBLE_LESS", "AT_SIGN", "TILDE", "BANG", "PERCENT", "KW_DEFAULT",
  "KW_CLASS", "KW_INSTANCE", "KW_DERIVING", "KW_IMPORT", "KW_MODULE",
  "KW_FOREIGN", "KW_INFIX", "KW_INFIXR", "KW_AS", "KW_HIDING",
  "KW_QUALIFIED", "KW_NEWTYPE", "KW_EXPORT", "KW_CCALL", "KW_PRINT",
  "KW_GETLINE", "KW_INT", "KW_INTEGER", "KW_CHAR", "KW_STRING", "KW_FLOAT",
  "KW_BOOL", "BACKTICK", "KW_INFIXL", "AMPERSAND", "OPERATOR", "LBRACE",
  "RBRACE", "DO_STMT_TERM", "APPLY_PREC", "$accept", "program",
  "decl_list", "decl_list_opt", "decl_block", "opt_where", "decl",
  "let_block", "binding_list", "binding_list_opt", "binding",
  "func_definition", "func_signature", "type_expr", "data_decl",
  "constr_list", "basic_expr", "app_expr", "pow_expr", "mult_expr",
  "add_expr", "comp_expr", "log_and_expr", "log_or_expr", "cons_expr",
  "compose_expr", "expr", "tuple_list", "arr_list", "atomic_pattern",
  "constructor_app", "pattern", "tuple_content", "pattern_list",
  "param_list", "case_branch_list_opt", "case_branch_list", "case_branch",
  "do_block", "do_stmt_list", "do_stmt", "semicolon_opt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   102,   103,   104,   104,   105,   105,   106,   107,   107,
     108,   108,   108,   108,   108,   109,   110,   110,   111,   111,
     112,   113,   114,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   116,   117,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   119,   120,   120,   120,   120,   121,   121,   121,
     121,   122,   122,   122,   123,   123,   123,   123,   123,   123,
     123,   124,   124,   125,   125,   126,   126,   127,   127,   127,
     127,   128,   128,   128,   128,   128,   128,   128,   129,   130,
     130,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     132,   132,   133,   133,   133,   134,   135,   135,   136,   136,
     137,   137,   138,   138,   139,   140,   141,   141,   142,   142,
     143,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     0,     3,     2,     0,
       1,     1,     1,     1,     1,     4,     2,     1,     1,     0,
       4,     5,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     4,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     2,
       2,     1,     2,     1,     3,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       3,     1,     4,     6,     6,     3,     2,     2,     3,     3,
       1,     1,     1,     1,     1,     2,     3,     2,     3,     3,
       1,     2,     1,     1,     3,     3,     3,     1,     2,     1,
       1,     0,     2,     3,     3,     3,     3,     1,     3,     1,
       1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,    12,    14,    11,
      10,    13,    93,    94,    91,   100,    92,     0,     0,     0,
       0,   103,   102,   109,     0,    19,     0,     1,     3,    29,
      28,     0,     0,    23,    25,    26,    24,    27,     0,    40,
      41,    44,    45,    36,    37,    38,    39,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    53,    57,
      61,    64,    71,    73,    75,    77,    81,   121,    95,     0,
       0,    97,   107,     0,   101,     0,     0,   108,     0,    18,
       0,    17,     0,     0,     0,     0,    22,     0,    86,     0,
       0,    87,     0,    49,    77,     0,     0,    50,    90,     0,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   120,    20,     0,    99,    96,     0,    98,   104,
       9,    16,    15,    35,    33,    31,    32,    30,   119,     0,
     117,     0,     0,     0,     0,    46,    47,     0,    48,    85,
      54,    55,    56,    58,    59,    60,    62,    63,    67,    68,
      69,    70,    65,    66,    72,    74,    76,    80,    79,    78,
     105,   106,     0,    21,     0,     0,     0,   115,   111,     0,
      82,    88,    89,     6,     8,    34,   118,   116,     0,     0,
     110,     0,     0,     5,     0,     0,    84,     0,   112,    83,
       7,   114,   113
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,   194,   184,   173,     6,    56,    79,    80,
       8,     9,    10,    38,    11,   134,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,   138,    96,    99,    21,
      22,    72,    70,    73,    24,   189,   190,   191,    88,   139,
     140,   123
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -79
static const yytype_int16 yypact[] =
{
      55,   169,   -18,    21,    44,    55,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,    10,   111,   196,
      27,   -79,   183,    15,   205,    66,    96,   -79,   -79,   -79,
     -79,    10,    10,   -79,   -79,   -79,   -79,   -79,    34,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,    42,
     111,   111,   111,   111,   159,   101,    67,   -79,    45,   -79,
     -20,   202,   -79,    32,    64,   -79,   -14,   100,   -79,    47,
      98,   -79,    57,    99,   -79,   210,   111,    15,   112,    66,
     102,   -79,   134,    -8,   -12,    10,   -79,   111,   -79,   127,
     124,   -79,   122,   -79,   117,   -17,   125,   -79,   149,   140,
     111,   147,   147,   147,   -79,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,    10,
     147,   147,   -79,   -79,   210,   -79,   -79,   210,   -79,    15,
     171,   -79,   -79,   -79,   162,   -79,   -79,   180,   181,   -11,
     -79,   185,   111,   111,   147,   -79,   -79,   147,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -20,   -20,    52,    52,
      52,    52,    52,    52,   -79,   -79,   -79,   -79,   -14,   -79,
     -79,   -79,   189,   -79,   213,   111,   111,   -79,   210,   208,
     -79,   -79,   -79,    55,   -79,   -79,   -79,   -79,   -13,   190,
     210,   203,   111,    55,   195,   111,   -79,   204,   -79,   -79,
     -79,   -79,   -79
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -79,   -79,    56,   -79,   -79,   -79,    -4,     0,   -79,   -79,
     -21,   -79,   -79,   -22,   -79,   -79,   188,   -79,   120,    23,
     146,   132,   -79,   121,   -48,   -46,   -15,   -79,   -78,   219,
     -79,     1,   -79,   -52,   -79,   -79,   -79,    59,   -79,   -79,
      86,   -79
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       7,    28,    23,    67,    81,     7,    94,    98,    95,    83,
      84,   119,   195,    85,   119,    29,    30,    85,    75,   145,
      69,    25,   176,   105,   106,    77,   136,    26,   135,   177,
      12,    13,    14,    15,    16,    89,    90,    91,    92,   120,
     121,   107,   120,   121,    27,    31,    75,    32,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,   131,    85,
       1,   130,    19,   137,    20,    71,   181,    86,     2,   182,
     166,    78,   170,   169,   168,   171,   129,     3,    75,   124,
      54,    87,    55,   125,   100,   149,   116,   117,    75,   127,
     101,   102,   103,   108,   109,   118,    98,   167,    33,    98,
      34,    35,    36,    37,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,     2,    82,    49,   179,   180,    50,
      51,   156,   157,   122,   126,    52,    54,   128,    55,    97,
     133,    18,   132,   141,   142,    53,    54,   143,    55,   144,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
     186,   146,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    12,    13,    14,    15,    16,   199,   148,   188,
     201,   147,    54,     7,    55,   172,    12,    13,    14,    28,
      16,   188,   174,     7,    54,    93,    55,    17,    18,    12,
      13,    14,    15,    16,    19,    85,    20,   175,    12,    13,
      14,    15,    16,    12,    13,    14,    15,    16,    19,   185,
      20,   150,   151,   152,   178,   153,   154,   155,   183,   192,
     196,    19,    68,    20,    76,   200,   198,   202,   165,   193,
      19,    74,    20,   108,   109,    19,   104,    20,   164,   197,
     110,   111,   112,   113,   114,   115,   158,   159,   160,   161,
     162,   163,   187
};

static const yytype_uint8 yycheck[] =
{
       0,     5,     1,    18,    25,     5,    54,    55,    54,    31,
      32,    28,    25,    25,    28,     5,     6,    25,    31,    36,
      19,    39,    33,    43,    44,    24,    38,     6,    36,    40,
       3,     4,     5,     6,     7,    50,    51,    52,    53,    56,
      57,    61,    56,    57,     0,    35,    31,    37,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    79,    25,
       5,    76,    35,    85,    37,    38,   144,    33,    13,   147,
     118,     5,   124,   121,   120,   127,    75,    22,    31,    32,
      35,    39,    37,    36,    17,   100,    54,    55,    31,    32,
      45,    46,    47,    41,    42,    31,   144,   119,    88,   147,
      90,    91,    92,    93,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    29,    15,   142,   143,    18,
      19,   108,   109,    33,    36,    24,    35,    38,    37,    38,
       6,    29,    40,    16,    20,    34,    35,    25,    37,    32,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
     175,    36,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,     3,     4,     5,     6,     7,   192,    38,   178,
     195,    32,    35,   183,    37,    14,     3,     4,     5,   193,
       7,   190,    30,   193,    35,    36,    37,    28,    29,     3,
       4,     5,     6,     7,    35,    25,    37,    26,     3,     4,
       5,     6,     7,     3,     4,     5,     6,     7,    35,     6,
      37,   101,   102,   103,    39,   105,   106,   107,    39,    21,
      40,    35,    36,    37,    29,    40,    33,    33,   117,   183,
      35,    22,    37,    41,    42,    35,    58,    37,   116,   190,
      48,    49,    50,    51,    52,    53,   110,   111,   112,   113,
     114,   115,   176
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    13,    22,   103,   104,   108,   109,   112,   113,
     114,   116,     3,     4,     5,     6,     7,    28,    29,    35,
      37,   131,   132,   133,   136,    39,     6,     0,   108,     5,
       6,    35,    37,    88,    90,    91,    92,    93,   115,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    15,
      18,    19,    24,    34,    35,    37,   109,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    36,   133,
     134,    38,   133,   135,   131,    31,    29,   133,     5,   110,
     111,   112,    29,   115,   115,    25,    33,    39,   140,   128,
     128,   128,   128,    36,   126,   127,   129,    38,   126,   130,
      17,    45,    46,    47,   118,    43,    44,    61,    41,    42,
      48,    49,    50,    51,    52,    53,    54,    55,    31,    28,
      56,    57,    33,   143,    32,    36,    36,    32,    38,   133,
     128,   112,    40,     6,   117,    36,    38,   115,   128,   141,
     142,    16,    20,    25,    32,    36,    36,    32,    38,   128,
     120,   120,   120,   120,   120,   120,   121,   121,   122,   122,
     122,   122,   122,   122,   123,   125,   126,   115,   127,   126,
     135,   135,    14,   107,    30,    26,    33,    40,    39,   128,
     128,   130,   130,    39,   106,     6,   128,   142,   133,   137,
     138,   139,    21,   104,   105,    25,    40,   139,    33,   128,
      40,   128,    33
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 105 "parser.y"
    { root = ProgramNode::create((yyvsp[(1) - (1)].decl_list)); (yyval.program) = root; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 108 "parser.y"
    { (yyval.decl_list) = DeclListNode::addDecl((yyvsp[(1) - (2)].decl_list), (yyvsp[(2) - (2)].decl)); ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 109 "parser.y"
    { (yyval.decl_list) = DeclListNode::create((yyvsp[(1) - (1)].decl)); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 113 "parser.y"
    { (yyval.decl_list) = (yyvsp[(1) - (1)].decl_list); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 114 "parser.y"
    { (yyval.decl_list) = nullptr; ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 118 "parser.y"
    { (yyval.decl_list) = (yyvsp[(2) - (3)].decl_list); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 136 "parser.y"
    { (yyval.decl) = DeclNode::createLetBlock((yyvsp[(3) - (4)].decl_list)); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 140 "parser.y"
    { (yyval.decl_list) = DeclListNode::addDecl((yyvsp[(1) - (2)].decl_list), (yyvsp[(2) - (2)].decl)); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 141 "parser.y"
    { (yyval.decl_list) = DeclListNode::create((yyvsp[(1) - (1)].decl)); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 145 "parser.y"
    { (yyval.decl_list) = (yyvsp[(1) - (1)].decl_list); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 146 "parser.y"
    { (yyval.decl_list) = nullptr; ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 150 "parser.y"
    { (yyval.decl) = DeclNode::createVarDecl((yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 156 "parser.y"
    { (yyval.decl) = DeclNode::createFuncDef((yyvsp[(1) - (5)].str), (yyvsp[(2) - (5)].decl_list), (yyvsp[(4) - (5)].expr), (yyvsp[(5) - (5)].decl_list)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 160 "parser.y"
    { (yyval.decl) = DeclNode::createFuncSignature((yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].expr)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 164 "parser.y"
    { (yyval.expr) = ExprNode::createPrimitiveType("Int"); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 165 "parser.y"
    { (yyval.expr) = ExprNode::createPrimitiveType("Float"); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 166 "parser.y"
    { (yyval.expr) = ExprNode::createPrimitiveType("Char"); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 167 "parser.y"
    { (yyval.expr) = ExprNode::createPrimitiveType("String"); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 168 "parser.y"
    { (yyval.expr) = ExprNode::createPrimitiveType("Bool"); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 169 "parser.y"
    { (yyval.expr) = ExprNode::createTypeConstructor((yyvsp[(1) - (1)].str)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 170 "parser.y"
    { (yyval.expr) = ExprNode::createTypeVar((yyvsp[(1) - (1)].str)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 171 "parser.y"
    { (yyval.expr) = ExprNode::createFunctionType((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 172 "parser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 173 "parser.y"
    { (yyval.expr) = ExprNode::createListType((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 178 "parser.y"
    { (yyval.decl) = DeclNode::createDataDecl((yyvsp[(2) - (4)].str), (yyvsp[(4) - (4)].decl_list)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 182 "parser.y"
    { (yyval.decl_list) = DeclListNode::addConstructorToList((yyvsp[(1) - (3)].decl_list), (yyvsp[(3) - (3)].str)); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 183 "parser.y"
    { (yyval.decl_list) = DeclListNode::createConstructorList((yyvsp[(1) - (1)].str)); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 200 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].str)); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 201 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].str)); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 202 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].str)); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 203 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].str)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 204 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral("True"); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 205 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral("False"); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 206 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].strVal)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 207 "parser.y"
    { (yyval.expr) = ExprNode::createLiteral((yyvsp[(1) - (1)].strVal)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 208 "parser.y"
    { (yyval.expr) = ExprNode::createVarRef((yyvsp[(1) - (1)].str)); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 209 "parser.y"
    { (yyval.expr) = ExprNode::createVarRef((yyvsp[(1) - (1)].str)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 210 "parser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 211 "parser.y"
    { (yyval.expr) = ExprNode::createTupleExpr((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 212 "parser.y"
    { (yyval.expr) = ExprNode::createArrayExpr((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 213 "parser.y"
    { (yyval.expr) = nullptr; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 214 "parser.y"
    { (yyval.expr) = nullptr; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 218 "parser.y"
    { (yyval.expr) = ExprNode::createFuncCall((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 226 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("^", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 227 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("**", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 228 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("^^", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 236 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("*", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 237 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("/", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 238 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("!!", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 246 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("+", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 247 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("-", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 255 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("==", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 256 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("!=", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 257 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("<", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 258 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr(">", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 259 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("<=", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 260 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr(">=", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 268 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("&&", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 272 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("||", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 279 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr(":", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 286 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr(".", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 287 "parser.y"
    { (yyval.expr) = ExprNode::createBinaryExpr("$", (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 288 "parser.y"
    { (yyval.expr) = ExprNode::createTypeAnnotation((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 296 "parser.y"
    { (yyval.expr) = ExprNode::createLambda((yyvsp[(2) - (4)].expr), (yyvsp[(4) - (4)].expr)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 299 "parser.y"
    { (yyval.expr) = ExprNode::createIfExpr((yyvsp[(2) - (6)].expr), (yyvsp[(4) - (6)].expr), (yyvsp[(6) - (6)].expr)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 300 "parser.y"
    { (yyval.expr) = ExprNode::createCaseExpr((yyvsp[(2) - (6)].expr), (yyvsp[(5) - (6)].expr)); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 303 "parser.y"
    { (yyval.expr) = ExprNode::createLetInExpr((yyvsp[(1) - (3)].decl), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 306 "parser.y"
    { (yyval.expr) = ExprNode::createDoExpr((yyvsp[(2) - (2)].expr)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 309 "parser.y"
    { (yyval.expr) = ExprNode::createReturnExpr((yyvsp[(2) - (2)].expr)); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 313 "parser.y"
    { (yyval.expr) = ExprNode::addExprToList((yyvsp[(3) - (3)].expr), (yyvsp[(1) - (3)].expr)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 315 "parser.y"
    { (yyval.expr) = ExprNode::addExprToList((yyvsp[(3) - (3)].expr), (yyvsp[(1) - (3)].expr)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 316 "parser.y"
    { (yyval.expr) = ExprNode::createExprList((yyvsp[(1) - (1)].expr)); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 325 "parser.y"
    { (yyval.expr) = ExprNode::createVarPattern((yyvsp[(1) - (1)].str)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 326 "parser.y"
    { (yyval.expr) = ExprNode::createLiteralPattern((yyvsp[(1) - (1)].str)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 327 "parser.y"
    { (yyval.expr) = ExprNode::createLiteralPattern("True"); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 328 "parser.y"
    { (yyval.expr) = ExprNode::createLiteralPattern("False"); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 329 "parser.y"
    { (yyval.expr) = nullptr; ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 330 "parser.y"
    { (yyval.expr) = ExprNode::createTuplePattern((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 331 "parser.y"
    { (yyval.expr) = nullptr; ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    { (yyval.expr) = ExprNode::createListPattern((yyvsp[(2) - (3)].expr)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 333 "parser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 336 "parser.y"
    { (yyval.expr) = ExprNode::createConstructorPattern((yyvsp[(1) - (1)].str), nullptr); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 337 "parser.y"
    { (yyval.expr) = ExprNode::addArgumentToConstructor((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 342 "parser.y"
    { (yyval.expr) = ExprNode::createConsPattern((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 345 "parser.y"
    { (yyval.expr) = ExprNode::addPatternToList((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 347 "parser.y"
    { (yyval.expr) = ExprNode::addPatternToList((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 348 "parser.y"
    { (yyval.expr) = ExprNode::createPatternList((yyvsp[(1) - (1)].expr)); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 354 "parser.y"
    { (yyval.decl_list) = DeclListNode::addParamToList((yyvsp[(1) - (2)].decl_list), (yyvsp[(2) - (2)].expr)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 355 "parser.y"
    { (yyval.decl_list) = DeclListNode::createParamList((yyvsp[(1) - (1)].expr)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 360 "parser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 361 "parser.y"
    { (yyval.expr) = nullptr; ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 366 "parser.y"
    { (yyval.expr) = ExprNode::createCaseBranchList((yyvsp[(1) - (2)].expr)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 368 "parser.y"
    { (yyval.expr) = ExprNode::addCaseBranchToList((yyvsp[(1) - (3)].expr), (yyvsp[(2) - (3)].expr)); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 372 "parser.y"
    { (yyval.expr) = ExprNode::createCaseBranch((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2348 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 395 "parser.y"


void yyerror(const char *s) {
    extern int yylineno;
    extern char *yytext;
    fprintf(stderr, "Parser error: %s at line %d, near token '%s'\n", s, yylineno, yytext);
}

