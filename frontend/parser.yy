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
  SLASH   "/"
  LPAREN  "("
  RPAREN  ")"
  SEMICOLON ";"
  LCBRACKET "{"
  RCBRACKET "}"
  COMMA ","
  TINT "int"
  RETURN "return"
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
//%nterm <int> exp
%nterm <std::unique_ptr<Expression>> exp
//%nterm <std::unique_ptr<Block>> block
%nterm <std::unique_ptr<Block>> linstructions
%nterm <std::unique_ptr<Instruction>> assignment
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


%printer { yyo << "Error---"; } <*>;

%%
%start program;

program:
  linstructions { drv.root = std::make_unique<Program>(std::move($1), std::move($1));}
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
;

function:
  type "identifier" "(" fparameters ")" blockf {$$ = std::make_unique<FunctionInst>(0,0,$2,std::move($4),std::move($6));}
;

blockf:
  "{" linstructionf "}" {$$=std::move($2);}
;

linstructionf: 
   %empty                        {$$= std::make_unique<Block>(0,0);}
  |linstructionf instructionf      {$$ = std::move($1); $$->addInstruction(std::move($2));}
;

instructionf:
  instruction {$$=std::move($1);}
| returni { $$ = std::move($1);}
;

returni:
  "return" exp ";" {$$=std::make_unique<ReturnInst>(0,0,std::move($2));}
;

type:
  "identifier" {$$=std::make_unique<Type>(TCLASS, $1);}
  | "tint" {$$ = std::make_unique<Type>(TINT);}
;

fparameters: 
    %empty {$$=std::make_unique<FormalParameters>();}
  | fparameters "," fparameter {$$=std::move($1); $$->addParameter(std::move($3));}
;

fparameter:
  type "identifier" { $$= std::make_unique<Parameter>(0,0,std::move($1),$2);}
;

assignment:
  "identifier" "=" exp ";"{ $$= std::make_unique<Assignation>(0,0,std::move($1),std::move($3));}
  ;

%left "+" "-";
%left "*" "/";
%left "**";

exp:
  "number"      { $$ = std::make_unique<IntExp>(0,0,std::move($1));}
| "identifier"  { $$ = std::make_unique<VarExp>(0,0,std::move($1));}
| exp "+" exp   { $$ = std::make_unique<AddExp>(0,0,std::move($1),std::move($3));}
| exp "-" exp   { $$ = std::make_unique<SubExp>(0,0,std::move($1),std::move($3));}
| exp "*" exp   { $$ = std::make_unique<MulExp>(0,0,std::move($1),std::move($3));}
| exp "/" exp   { $$ = std::make_unique<DivExp>(0,0,std::move($1),std::move($3));}
| "(" exp ")"   { $$ = std::move($2); }
;
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
