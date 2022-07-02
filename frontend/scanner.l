%{
    #include "parser.h"
    #include "symbol.h"
    #include <iostream>
    #include <stdio.h>
    #include <string.h>
    int n_lines;


    void printTokenInfo()
    {
        if(yylineno==1)
        {
            std::cout << yylineno << ")" << yytext << endl;
        }
    }

%}

%x STATE1 STATE2
%option noyywrap
%option yylineno

DIGIT [0-9]
ID ([a-zA-Z]|_)([a-zA-Z]|_|{DIGIT})*
STRING "\""[^"\""]*"\""
%%


[\n]+               {n_lines+= yyleng;}
{DIGIT}+            yyval.int_value = atoi(yytext); return INTEGER;
{DIGIT}+"."{DIGIT}* yyval.double_value = atof(yytext); return DOUBLE;
"<="                return LESSEQUAL;
">="                return GREATEREQUEAL;
"=="                return EQUAL;
"!="                return DIFFERENT;
"&&"                return AND;
"||"                return OR;
true                yyval.boolean_value = true; return TRUE;
false               yyval.boolean_value = false; return FALSE;
begin               return BEGIN;
void                return VOID;
int                 return TYPE_INT;
double              return TYPE_DOUBLE;
char                return TYPE_CHAR;
String              return TYPE_STRING;
print               return PRINT;
{ID}                strcpy(yylval.string_value,yytext);   return ID;
{STRING}            {strcpy(yylval.string_value,&yytext[1]); yylval.valor_cadena[yyleng-2] = '\0';   return STRING;}
[\t ]               ;
<<EOF>>             yyterminate();
.                   return (yytext[0]);
