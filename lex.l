%{
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <deque>
#include "token_queue.h"
#include "parser.tab.h"

using namespace std;

void push_token(int type, const string& text);
void lexical_error(const char* message, const char* value);
int count_last_line_indent(const char* text); 
bool process_indentation(const char* line_text, vector<int>& layout_stack, bool start_block_flag, bool first_block_indent_flag, bool end_of_file_flag);
void place_semicolons(const char* line_text, int first_indent, bool end_of_file_flag);
%}

%x STRING CHAR ESCAPE COMMENT MULTILINE_COMMENT AFTER_KEYWORD FIRST_LINE

%option never-interactive
%option noyywrap
%option yylineno

DEC         [0-9]
HEX         0[xX][0-9a-fA-F]+
OCT         0[oO][0-7]+
LETTER      [a-zA-Z]
ID_START    [a-z_]
CAPITAL     [A-Z]
WHITESPACE  [ ]
HORIZ_TAB   [\t]
NEWLINE     [\n]
OPERATOR    [\+\-\*\/\=\<\>\&\|\^\!\~\\\@\#\$\%\?\.\:]

%%

%{
string string_buffer;        // для строковых литералов
bool is_char_escape;         // флаг для определения типа escape-последовательности
string comment_buffer;	     // для комментариев
int comment_nest_level = 0;  // для отслеживания вложенности комментариев
bool char_processed = false; // для отслеживания обработки char
vector<int> layout_stack = {0};   // стек с количествами отступов  
bool indent_process_flag = false; // флаг обработки отступов в блоках
int first_indent = 0;		 // количество отступов первой строки
bool first_line_flag = true; // флаг необходимости обработки первой строки отдельно
int brace_count = 0;		 // счетчик для баланса фигурных скобок
bool first_block_indent_flag = false;
bool custom_indents_in_block_flag = false; // флаг пропуска блока с пользовательскими отступами
%}

	/* Проверяем, находимся ли мы в начале файла */
	if (first_line_flag) {
		first_line_flag = false;
		BEGIN(FIRST_LINE);
	}
	
<FIRST_LINE>{
^[\n \t]*		{ first_indent = count_last_line_indent(yytext); 
				  BEGIN(INITIAL); 
				}

.    			{ 
				yyless(0);
				first_indent = 0;
				BEGIN(INITIAL);
				}
}

{WHITESPACE}+				  { /* Пропускаем пробелы */ }
{HORIZ_TAB}+	              {  /* Пропускаем табы */ }

"--"            {
                  // Однострочный комментарий
                  comment_buffer.clear();
                  comment_buffer = "--";
                  BEGIN(COMMENT);
                }

<COMMENT>{
  \n            { 
				  yyless(0);
                  push_token(SINGLELINE_COMMENT, comment_buffer);
                  BEGIN(INITIAL); 
                }
  .+            { 
                  comment_buffer += yytext;
                }
  <<EOF>>       {
                  push_token(SINGLELINE_COMMENT, comment_buffer);
                  BEGIN(INITIAL);
                }
}

"{-"            { 
                  // Начало многострочного комментария
                  comment_nest_level = 1;
                  comment_buffer.clear();
                  comment_buffer = "{-";
                  BEGIN(MULTILINE_COMMENT);
                }

<MULTILINE_COMMENT>{
  "-"+"}"       { 
                  comment_buffer += yytext;
                  push_token(MULTILINE_COMMENT, comment_buffer);
                  BEGIN(INITIAL);
                }
				
    [^-]*       { comment_buffer += yytext; } 
    "-"+[^-}]*  { comment_buffer += yytext; }
				
    <<EOF>>     { 
                  lexical_error("Unterminated multiline comment", comment_buffer.c_str());
			   BEGIN(INITIAL);
                }
}

"->"           { push_token(RIGHT_ARROW, yytext); }
"<-"           { push_token(LEFT_ARROW, yytext); }
"=>"           { push_token(FAT_ARROW, yytext); }
"::"           { push_token(DOUBLE_COLON, yytext); }
"++"           { push_token(DOUBLE_PLUS, yytext); }
".."           { push_token(DOUBLE_DOT, yytext); }
"=="           { push_token(DOUBLE_EQUALS, yytext); }
"/="           { push_token(NOT_EQUALS, yytext); }
"<="           { push_token(LESS_EQUAL, yytext); }
">="           { push_token(GREATER_EQUAL, yytext); }
"&&"           { push_token(DOUBLE_AMPERSAND, yytext); }
"||"           { push_token(DOUBLE_PIPE, yytext); }
"!!"           { push_token(DOUBLE_BANG, yytext); }
".&."          { push_token(DOT_AMPERSAND_DOT, yytext); }
".|."          { push_token(DOT_PIPE_DOT, yytext); }
">>="          { push_token(DOUBLE_GREATER_EQUAL, yytext); }
">>"           { push_token(DOUBLE_GREATER, yytext); }
"=<<"          { push_token(EQUAL_DOUBLE_LESS, yytext); }
"^^"           { push_token(DOUBLE_CARET, yytext); }
"**"           { push_token(DOUBLE_ASTERISK, yytext); }
"$"            { push_token(DOLLAR, yytext); }
"."            { push_token(DOT, yytext); }
":"            { push_token(COLON, yytext); }
"="            { push_token(EQUALS, yytext); }
"\\"           { push_token(BACKSLASH, yytext); }
"|"            { push_token(PIPE, yytext); }
"@"            { push_token(AT_SIGN, yytext); }
"~"            { push_token(TILDE, yytext); }
"+"            { push_token(PLUS, yytext); }
"-"            { push_token(MINUS, yytext); }
"*"            { push_token(ASTERISK, yytext); }
"/"            { push_token(SLASH, yytext); }
"^"            { push_token(CARET, yytext); }
"<"            { push_token(LESS, yytext); }
">"            { push_token(GREATER, yytext); }
"!"            { push_token(BANG, yytext); }
"&"            { push_token(AMPERSAND, yytext); }
"%"            { push_token(PERCENT, yytext); }

"let"          { push_token(KW_LET, yytext); BEGIN(AFTER_KEYWORD);}
"where"        { push_token(KW_WHERE, yytext); BEGIN(AFTER_KEYWORD);}
"do"           { push_token(KW_DO, yytext); BEGIN(AFTER_KEYWORD);}
"of"           { push_token(KW_OF, yytext); BEGIN(AFTER_KEYWORD);}

<AFTER_KEYWORD>{
[\n \t]*\{ 		{
				 yyless(0);
				 indent_process_flag = false;
				 custom_indents_in_block_flag = true;
				 BEGIN(INITIAL);
				}
[\n \t]*[^{]	{
				 yyless(0);
				 indent_process_flag = true;
				 custom_indents_in_block_flag = false;
				 process_indentation(yytext, layout_stack, true, first_block_indent_flag, false);
				 first_block_indent_flag = true;
				 BEGIN(INITIAL);
				}
}

\n+[ \t\n]* {
				if (indent_process_flag && !custom_indents_in_block_flag){
					indent_process_flag = process_indentation(yytext, layout_stack, false, first_block_indent_flag, false);
					first_block_indent_flag = false;
				} else if (!custom_indents_in_block_flag){
					place_semicolons(yytext, first_indent, false);
				}
            }
		   
<INITIAL>{
<<EOF>>    {
				if (indent_process_flag && !custom_indents_in_block_flag){
					while (indent_process_flag){
						indent_process_flag = process_indentation(yytext, layout_stack, false, first_block_indent_flag, true);
						first_block_indent_flag = false;
					}
				} else if (!custom_indents_in_block_flag){
					place_semicolons(yytext, first_indent, true);
				}
				
				if (brace_count != 0){
					lexical_error("Mismatched brackets", yytext);
				}
				return 0;
           }
}		   	   


"default"	     { push_token(KW_DEFAULT, yytext); }
"in"           { push_token(KW_IN, yytext); }
"return"       { push_token(KW_RETURN, yytext); }
"case"         { push_token(KW_CASE, yytext); }
"if"           { push_token(KW_IF, yytext); }
"then"         { push_token(KW_THEN, yytext); }
"else"         { push_token(KW_ELSE, yytext); }
"data"         { push_token(KW_DATA, yytext); }
"type"         { push_token(KW_TYPE, yytext); }
"class"        { push_token(KW_CLASS, yytext); }
"instance"     { push_token(KW_INSTANCE, yytext); }
"deriving"     { push_token(KW_DERIVING, yytext); }
"import"       { push_token(KW_IMPORT, yytext); }
"module"       { push_token(KW_MODULE, yytext); }
"foreign"      { push_token(KW_FOREIGN, yytext); }
"infixl"       { push_token(KW_INFIXL, yytext); }
"infixr"       { push_token(KW_INFIXR, yytext); }
"infix"        { push_token(KW_INFIX, yytext); }
"as"           { push_token(KW_AS, yytext); }
"hiding"       { push_token(KW_HIDING, yytext); }
"qualified"    { push_token(KW_QUALIFIED, yytext); }
"newtype"      { push_token(KW_NEWTYPE, yytext); }
"export"	     { push_token(KW_EXPORT, yytext); }
"ccall"        { push_token(KW_CCALL, yytext); }
"print"        { push_token(KW_PRINT, yytext); }
"getLine"      { push_token(KW_GETLINE, yytext); }

"Int"          { push_token(KW_INT, yytext); }
"Integer"      { push_token(KW_INTEGER, yytext); }
"Char"         { push_token(KW_CHAR, yytext); }
"String"       { push_token(KW_STRING, yytext); }
"Float"        { push_token(KW_FLOAT, yytext); }
"Bool"         { push_token(KW_BOOL, yytext); }
"True"         { push_token(KW_TRUE, yytext); }
"False"        { push_token(KW_FALSE, yytext); }

"("            { push_token(LEFT_PAREN, yytext); }
")"            { push_token(RIGHT_PAREN, yytext); }
"["            { push_token(LEFT_BRACKET, yytext); }
"]"            { push_token(RIGHT_BRACKET, yytext); }
"{"            { 
                 brace_count++;
                 push_token(LEFT_BRACE, yytext);
               }
"}"            { 
                 brace_count--;
				 if (brace_count == 0){
					custom_indents_in_block_flag = false;
				 }
                 push_token(RIGHT_BRACE, yytext);
               }
","            { push_token(COMMA, yytext); }
";"            { push_token(SEMICOLON, yytext); }
"`"            { push_token(BACKTICK, yytext); }

{OPERATOR}+    { push_token(OPERATOR, yytext); }

{ID_START}({LETTER}|{DEC}|_|')*   { push_token(ID, yytext); }
{CAPITAL}({LETTER}|{DEC}|_|')*    { push_token(ID_CAP, yytext); }
    
{DEC}+                            { push_token(DEC_LITERAL, yytext); }
{HEX}                             { push_token(HEX_LITERAL, yytext); }
{OCT}                             { push_token(OCT_LITERAL, yytext+2); }
{DEC}+\.{DEC}+([eE][+-]?{DEC}+)?  { push_token(FLOAT, yytext); }

\'     { string_buffer.clear(); is_char_escape = true; BEGIN(CHAR); }
\"     { string_buffer.clear(); is_char_escape = false; BEGIN(STRING); }

<CHAR,STRING>{
  \\\\ 		{
				/* Экранированный бекслеш */
				string_buffer += '\\';
			}

  \\[ \t\n\r\f\v]+\\ {
						/* Haskell string gap: backslash + whitespace 
						(включая перевод строки) + backslash */
						/* Игнорируем — ничего не добавляем в строку */
					}
  
  \\      { BEGIN(ESCAPE); }
  
  [^\\'\n"]+ { string_buffer += yytext; }

  \'      { 
            if (is_char_escape) {
                if (string_buffer.size() != 1) 
                    lexical_error("Invalid char literal length", string_buffer.c_str());
                push_token(CHAR_LITERAL, string_buffer);
                string_buffer.clear();
                BEGIN(INITIAL);
            } else {
                string_buffer += '\''; // обычный символ внутри строки
            }
          }

  \"      { 
            if (!is_char_escape) {
                push_token(STRING_LITERAL, string_buffer);
                string_buffer.clear();
                BEGIN(INITIAL);
            } else {
                string_buffer += '\"'; // обычный символ внутри char
            }
          }

  \n      { lexical_error("Unterminated literal", yytext); BEGIN(INITIAL); }

}

<ESCAPE>{
  n       { string_buffer += '\n'; BEGIN(is_char_escape ? CHAR : STRING); }
  t       { string_buffer += '\t'; BEGIN(is_char_escape ? CHAR : STRING); }
  r       { string_buffer += '\r'; BEGIN(is_char_escape ? CHAR : STRING); }
  a       { string_buffer += '\a'; BEGIN(is_char_escape ? CHAR : STRING); }
  b       { string_buffer += '\b'; BEGIN(is_char_escape ? CHAR : STRING); }
  f       { string_buffer += '\f'; BEGIN(is_char_escape ? CHAR : STRING); }
  v       { string_buffer += '\v'; BEGIN(is_char_escape ? CHAR : STRING); }
  &       { string_buffer += '\&'; BEGIN(is_char_escape ? CHAR : STRING); }
  0       { string_buffer += '\0'; BEGIN(is_char_escape ? CHAR : STRING); }
  '       { string_buffer += '\''; BEGIN(is_char_escape ? CHAR : STRING); }
  \"      { string_buffer += '\"'; BEGIN(is_char_escape ? CHAR : STRING); }
  \\      { string_buffer += '\\'; BEGIN(is_char_escape ? CHAR : STRING); }
  \?      { string_buffer += '\?'; BEGIN(is_char_escape ? CHAR : STRING); }

  [0-9]+  {
             int val = strtol(yytext, NULL, 10);
             if (val > 255) {
               lexical_error("Decimal escape sequence out of range", yytext);
               val = 255;
             }
             string_buffer += (char)val;
             BEGIN(is_char_escape ? CHAR : STRING);
           }

  o[0-7]+ {
             int val = strtol(yytext + 1, NULL, 8);
             if (val > 255) {
               lexical_error("Octal escape sequence out of range", yytext);
               val = 255;
             }
             string_buffer += (char)val;
             BEGIN(is_char_escape ? CHAR : STRING);
           }

  x[0-9a-fA-F]+ {
             int val = strtol(yytext + 1, NULL, 16);
             if (val > 255) {
               lexical_error("Hex escape sequence out of range", yytext);
               val = 255;
             }
             string_buffer += (char)val;
             BEGIN(is_char_escape ? CHAR : STRING);
           }

  \^[A-Z@\[\]\\\^_] {
             char c = yytext[1];
             char escaped_char = (c - 'A' + 1);
             string_buffer += escaped_char;
             BEGIN(is_char_escape ? CHAR : STRING);
           }

  .       {
             lexical_error("Unknown escape sequence", yytext);
             string_buffer += '\\';
             string_buffer += yytext;
             BEGIN(is_char_escape ? CHAR : STRING);
           }
}

<*>.         { lexical_error("Unknown symbol", yytext); }

%%

void push_token(int type, const string& text) {
    cout << "Pushed token: " << type << " -> '" << text << "'" << endl;
    token_queue.push_back({type, text});
}

void lexical_error(const char* message, const char* value) {
    cerr << "ERROR (line " << yylineno << "): " << message << ": '" << value << "'" << endl;
}

void place_semicolons(const char* line_text, int first_indent, bool end_of_file_flag) {
	int current_indent = count_last_line_indent(line_text);
	if (current_indent == first_indent) {
		// Тот же отступ - разделитель выражений
		push_token(SEMICOLON, ";");
	} else if (current_indent < first_indent && !end_of_file_flag) {
		lexical_error("Indentation without context", line_text);
	}
}

int count_last_line_indent(const char* text) {
    const char* last_line_start = text;
    
    // Находим начало последней строки
    while (*text) {
        if (*text == '\n') {
            last_line_start = text + 1;
        }
        text++;
    }
    
    // Считаем отступ для последней строки
    int indent = 0;
    while (*last_line_start == ' ' || *last_line_start == '\t') {
        if (*last_line_start == '\t') {
            indent += 8;
        } else {
            indent++;
        }
        last_line_start++;
    }
    return indent;
}

bool process_indentation(const char* line_text, vector<int>& layout_stack, bool start_block_flag, bool first_block_indent_flag, bool end_of_file_flag) {
	bool add_indent_flag = false; // флаг для определения необходимости сохранения отступа после "{"
	
	// Если вошли сразу после ключевого слова, то выводим токен с "{"
	if (start_block_flag) {
		push_token(LEFT_BRACE, "{");
		add_indent_flag = true;
		return true;
	} 
	
	int current_indent = count_last_line_indent(line_text); // получаем текущий отступ в строке
	
	//Если отступ меньше отступа остального кода, то выводим ошибку
	if (current_indent < layout_stack[0] && !end_of_file_flag){
		lexical_error("Indentation without context", line_text);
	}
	
	// Если в предыдущем входе в функцию был выведен токен с "{", то сохраняем отступ в стеке
	if(first_block_indent_flag){
		layout_stack.push_back(current_indent);
	}
	
	int last_indent = first_block_indent_flag ? *(layout_stack.rbegin() + 1) : layout_stack.back();
	if(first_block_indent_flag)
	{
		if(current_indent == last_indent && !end_of_file_flag){
			push_token(SEMICOLON, ";");
			push_token(RIGHT_BRACE, "}");
			layout_stack.pop_back();
		}
		else if(current_indent <= last_indent && end_of_file_flag)
		{
			push_token(SEMICOLON, ";");
			while (layout_stack.size() > 1 && current_indent <= last_indent) 
			{
				layout_stack.pop_back();
				push_token(RIGHT_BRACE, "}");
				last_indent = layout_stack.back();
			}
		}
		else if(!token_queue.empty() && current_indent > last_indent)
		{
			if (token_queue.back().type != LEFT_BRACE){
				push_token(SEMICOLON, ";");
			}
		}
	}
	else
	{
		if(current_indent == last_indent)
		{
			push_token(SEMICOLON, ";");
		}
		else if(current_indent > last_indent)
		{
			lexical_error("Inconsistent indentation in block after key word", line_text);
		}
		else if(current_indent < last_indent || end_of_file_flag)
		{
			push_token(SEMICOLON, ";");
			while (layout_stack.size() > 1 && current_indent < last_indent) 
			{
				layout_stack.pop_back();
				push_token(RIGHT_BRACE, "}");
				last_indent = layout_stack.back();
			}
		}
	}

	return (layout_stack.size() > 1);
}

int get_next_token(void) {
    if(!token_queue.empty()) {
        Token t = token_queue.front();
        token_queue.pop_front();
        yylval.str = strdup(t.text.c_str()); 
        printf("Parser reads token: %d ('%s')\n", t.type, t.text.c_str());
        return(t.type);
    }

    #undef yylex
    int flex_result = yylex(); 
    #define yylex get_next_token 
    
    printf("Parser calls original yylex, returned %d\n", flex_result);
    
    if(flex_result == 0) {
        if (token_queue.empty()) {
            return 0;
        }
        return(get_next_token());
    }
    if(!token_queue.empty()) {
        return(get_next_token());
    }
    print_token_queue();
    return(flex_result); 
}