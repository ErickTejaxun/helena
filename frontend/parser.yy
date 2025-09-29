/* Parser for calc++.   -*- C++ -*-

   Copyright (C) 2005-2015, 2018-2021 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8"
%header

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  # include <cmath>
  # include "AST.h"
  class driver;
}

// The parsing context.
%param { driver& drv }
//%param { std::unique_ptr<Instruction> root }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hh"
# include "AST.h"
}

%define api.token.prefix {TOK_}
%token
  ASSIGN  "="
  MINUS   "-"
  PLUS    "+"
  POWER   "**"
  STAR    "*"
  DIV    "/"
  EQ      "=="
  GT      ">"
  GET      ">="
  LT      "<"
  LET      "<="
  LPAREN  "("
  RPAREN  ")"
  SEMICOLON ";"
  LCBRACKET "{"
  RCBRACKET "}"
  LBRACKET "["
  RBRACKET "]"  
  COMMA ","
  TINT "int"
  TDOUBLE "double"
  TSTRING "string"
  TBOOL "bool"
  RETURN "return"
  PRINT "print"
  WHILE "while"
  NEW "new"
  BREAK "break"
  CONTINUE "continue"
  TRUE "true"
  FALSE "false"
;

%token <std::string> IDENTIFIER "identifier"
%token <std::string> STRING "stringchar"
%token <int> NUMBER "number"
%token <double> NUMBERD "decimal"
//%nterm <int> exp
%nterm <std::unique_ptr<Expression>> exp
//%nterm <std::unique_ptr<Block>> block
%nterm <std::unique_ptr<Block>> linstructions
%nterm <std::unique_ptr<Instruction>> assignment
%nterm <std::unique_ptr<Instruction>>aassignment
%nterm <std::unique_ptr<Instruction>> instruction
%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<FunctionInst>> function
%nterm <std::unique_ptr<Type>> type
%nterm <std::unique_ptr<Parameter>> fparameter
%nterm <std::unique_ptr<FormalParameters>> fparameters
%nterm <std::unique_ptr<Block>> blockf
%nterm <std::unique_ptr<Block>> linstructionf
%nterm <std::unique_ptr<Instruction>> returni 
%nterm <std::unique_ptr<Instruction>> instructionf
%nterm <std::unique_ptr<Instruction>> declaration
%nterm <std::unique_ptr<Instruction>> adeclaration
%nterm <std::unique_ptr<Instruction>> call
%nterm <std::unique_ptr<Instruction>> print
%nterm <std::unique_ptr<Instruction>> loopwhile
%nterm <std::unique_ptr<Instruction>> icontinue
%nterm <std::unique_ptr<Instruction>> ibreak

%printer { yyo << "Error---"; } <*>;

%%
%start program;

program:
  linstructions {        
      //std::cout << $1.get()->instructions.size() << std::endl;
      drv.root = std::make_unique<Program>(std::move($1));            
  }
;

//block: "{" linstructions "}" {$$ = std::move($2);}
//;

linstructions: 
  %empty                 {$$= std::make_unique<Block>(0,0);}
| linstructions instruction      {$$ = std::move($1); $$->addInstruction(std::move($2));}
;

instruction: 
  assignment { $$ = std::move($1);}
| function {$$ = std::move($1);}
| call { $$ = std::move($1);}
;

function:
  type "identifier" "(" fparameters ")" blockf {$$ = std::make_unique<FunctionInst>(0,0,std::move($1),$2,std::move($4),std::move($6));}
;

blockf:  
   "{" linstructionf "}" {$$=std::move($2);}
;

linstructionf: 
   %empty                        {$$= std::make_unique<Block>(0,0);}
  |linstructionf instructionf      {$$ = std::move($1); $$->addInstruction(std::move($2));}
;

instructionf:
  declaration { $$ = std::move($1);}
| adeclaration { $$= std::move($1);}
| assignment { $$= std::move($1);}
| aassignment { $$= std::move($1);}
| call { $$ = std::move($1);}
| returni { $$ = std::move($1);}
| print { $$ = std::move($1);}
| loopwhile {$$ = std::move($1);}
| icontinue {$$ = std::move($1);}
| ibreak {$$ = std::move($1);}
;

ibreak: 
  BREAK ";" { $$ = std::make_unique<BreakInst>(0,0);}
;

icontinue: 
  CONTINUE ";" { $$ = std::make_unique<ContinueInst>(0,0);}
;

loopwhile:
  WHILE "(" exp ")" blockf { $$= std::make_unique<While>(0,0,std::move($3), std::move($5));}
;

print :
  "print" "(" exp  ")" ";" { $$ = std::make_unique<PrintInstr>(0,0,std::move($3));}
;

call: 
  "identifier" "(" ")" ";"  { $$ = std::make_unique<CallInstr>(0,0,std::move($1));}
;

declaration: 
  type "identifier" "=" exp ";"  { $$=std::make_unique<Declaration>(0,0,std::move($1), $2,std::move($4));}
;

adeclaration:
  type "[" "]" "identifier" "=" NEW type "[" exp "]" ";" { $$=std::make_unique<DeclarationA>(0,0,std::move($1),$4,std::move($7),std::move($9));}  
;

returni:
  "return" exp ";" {$$=std::make_unique<ReturnInst>(0,0,std::move($2));}
;

type:
  TINT {$$ = std::make_unique<Type>(TINT);}
  | TDOUBLE {$$ = std::make_unique<Type>(TDOUBLE);}
  | TSTRING {$$ = std::make_unique<Type>(TSTRING);}
  | TBOOL {$$ = std::make_unique<Type>(TSTRING);}
;

fparameters: 
    %empty {$$=std::make_unique<FormalParameters>();}
  | fparameters "," fparameter {$$=std::move($1); $$->addParameter(std::move($3));}
;

fparameter:
  type "identifier" { $$= std::make_unique<Parameter>(0,0,std::move($1),$2);}
;

assignment:
  "identifier" "=" exp ";"{ $$= std::make_unique<Assignment>(0,0,std::move($1),std::move($3));}
  ;

aassignment:
  "identifier" "[" exp "]" "=" exp ";"{ $$= std::make_unique<ArrayAssignment>(0,0,std::move($1),std::move($3), std::move($6)); }
  ;  


%left "+" "-";
%left "*" "/";
%left "**";

exp:
   NUMBER      { $$ = std::make_unique<IntExp>(0,0,std::move($1));}
|  NUMBERD      { $$ = std::make_unique<DoubleExp>(0,0,std::move($1));}   
| "identifier"  { $$ = std::make_unique<VarExp>(0,0,std::move($1));}
| "stringchar"  { $$ = std::make_unique<StringExp>(0,0,$1);}
| exp PLUS exp   { $$ = std::make_unique<AddExp>(0,0,std::move($1),std::move($3));}
| exp MINUS exp   { $$ = std::make_unique<SubExp>(0,0,std::move($1),std::move($3));}
| exp STAR exp   { $$ = std::make_unique<MulExp>(0,0,std::move($1),std::move($3));}
| exp POWER exp  { $$ = std::make_unique<MulExp>(0,0,std::move($1),std::move($3));}
| exp DIV exp   { $$ = std::make_unique<DivExp>(0,0,std::move($1),std::move($3));}
| exp GT exp   { $$ = std::make_unique<CompExp>(0,0,std::move($1),std::move($3),1);}
| exp GET exp   { $$ = std::make_unique<CompExp>(0,0,std::move($1),std::move($3),2);}
| exp LT exp   { $$ = std::make_unique<CompExp>(0,0,std::move($1),std::move($3),3);}
| exp LET exp   { $$ = std::make_unique<CompExp>(0,0,std::move($1),std::move($3),4);}
| exp EQ exp   { $$ = std::make_unique<CompExp>(0,0,std::move($1),std::move($3),5);}
| "(" exp ")"   { $$ = std::move($2); }
| "identifier" "[" exp "]" { $$ = std::make_unique<VarArrayExp>(0,0,std::move($1),std::move($3));}
| TRUE           { $$ = std::make_unique<BooleanExp>(0,0,true);}
| FALSE          { $$ = std::make_unique<BooleanExp>(0,0,false);}
|
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
