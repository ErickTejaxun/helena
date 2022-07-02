%code requires
{
    #include "symbol.h"
}
%{
    #include <iostream>
    #include "symbol.h"
    #include <stdio.h>
    #include <string.h>
    #include <vector>
    #include <fstream>
    extern int n_lines;
    extern int yylex();
    extern FILE* yyin;
    extern FILE* yyout;
    AST *root;
    extern int yylineno;

    vector<Error> errors;
    Enviroment global = new Enviroment(NULL);

    void yyerror(const char* s)
    {
        errors.push_back(Error(n_lines, n_lines, string("Syntax"), string(s), string(s)));
    }
%}
%define parse.error verbose

%union
{
    int int_value;
    float double_value;
    char string_value[100];
    bool boolean_value;
    AST * node;    
}

%start start;

%token LESSEQUAL GREATEREQUAL EQUAL DIFFERENT AND OR
%token PRINT 
%token <boolean_value> TRUE FALSE
%token <int_value> INTEGER
%token <double_value> DOUBLE
%token <string_value> STRING ID

%type <node> program
%type <node> block
%type <node> expression
%type <node> print 
%type <node> instruction


%right '!'
%left AND OR 
%left '<' '>' LESSEQUAL GREATEREQUAL
%left '+' '-'
%left '%' '*' '/'
%right '^'
%left minus 
%nonassoc IF_SIMPLE
%nonassoc ELSE

%%

start: program {root = $1;}
;

program:
    block {
        $$ = new Programm(n_lines, n_lines);
        $$->setInstructions($1);
    }
;

block:
    block instruction {$$=$1; $$->addInstruction($2);}
    | instruction {$$ = new Block(n_lines, n_lines); $$->addInstruction($1);}
;

instruction:
    print {$$=$1;}
;

print: 
    PRINT '(' expression ')' ';' {$$= new print(n_lines, n_lines, $3);}
;

expression:
       INTEGER 		        {$$ = new Literal(n_lineas,n_lineas, $1); }
       | DOUBLE             {$$ = new Literal(n_lineas,n_lineas, $1); }
       | TRUE               {$$ = new Literal(n_lineas,n_lineas, $1); }
       | FALSE              {$$ = new Literal(n_lineas,n_lineas, $1); }
       | expression '+' expression          { $$ = new add(n_lineas, n_lineas, $1, $3); }
       | expression '-' expression    	    {$$ = new substract(n_lineas, n_lineas, $1, $3); }            
       | expression '*' expression          {$$ = new plus(n_lineas, n_lineas, $1,$3); } 
       | expression '/' expression          {$$ = new div(n_lineas, n_lineas, $1,$3); } 
       | expression '%' expression          {$$ = new module(n_lineas, n_lineas, $1,$3); } 
       | expression '^' expression          {$$ = new power(n_lineas, n_lineas, $1,$3); } 
       | expression '<' expression          {$$ = new relational(n_lineas, n_lineas, $1, "<", $3); }
       | expression '>' expression          {$$ = new relational(n_lineas, n_lineas, $1, ">", $3); }
       | expression EQUAL expression        {$$ = new relational(n_lineas, n_lineas, $1, "==", $3); }
       | expression DIFFERENT expression    {$$ = new relational(n_lineas, n_lineas, $1, "!=", $3); }
       | expression GREATEREQUAL expression {$$ = new relational(n_lineas, n_lineas, $1, ">=", $3); }
       | expression LESSEQUAL expression    {$$ = new relational(n_lineas, n_lineas, $1, "<=", $3); }
       | '!' expression                     {$$ = new negative(n_lineas,n_lineas,$2); }
       | expression AND expression          {$$ = new logic(n_lineas, n_lineas, $1, "&&", $3); }
       | expression OR expression           {$$ = new logic(n_lineas, n_lineas, $1, "||", $3); }
       |'-' expression %prec minus          {$$ = new minus(n_lineas,n_lineas,$2); }
       | '(' expression ')'                 {$$ = $2; }  
       | ID                   { string id($1);  $$ = new variable(n_lineas, n_lineas, id); }  
       | STRING               { string cad($1); $$ = new string(n_lineas, n_lineas, cad );}
       ;



