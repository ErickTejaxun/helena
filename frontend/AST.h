#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Value.h"

#include <string.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <list>
#include <memory>
#include <optional>

using namespace std;
class Error;

/* Global configuration's variables*/
extern vector<Error> errorList;

enum TYPES
{
    type_integer,
    type_real,
    type_logical,
    type_string,
    type_error
};

class Error
{
public:
    int line;
    int column;
    string description;
    string type;
    string id;
    Error(int l, int c, string t, string i, string desc) : line(l), column(c), type(t), description(desc), id(i)
    {
        line = l;
        column = c;
    }
    string getMessage()
    {
        return "Error " + type + " in line: " + to_string(line) + ":\t" + description;
    }

    static void logSemanticError(int l, int c, string i, string d)
    {
        errorList.push_back(Error(l, c, string("Semantic"), i, d));
    }
};

class ASTNode
{
public:
    //virtual void getLine() /* = defualt */;
    //virtual void getColumn() /* = defualt */;
    virtual ~ASTNode() = default;
    virtual llvm::Value *codgen() = 0;
};

class Expression : public ASTNode
{
public:
    //virtual void getValue() /* = defualt */;
    virtual ~Expression() = default;
};

class Instruction : public ASTNode
{
public:
    //virtual void execute() /* = defualt */;
    virtual ~Instruction() = default;
};



// Specific classes
class ArrayLevel
{
    std::list<std::unique_ptr<Expression>> levels;
    int line, column;

public:
    ArrayLevel(int line, int column) : line(line), column(column) {}

    bool addLevel(std::unique_ptr<Expression> expression)
    {
        if(expression)
        {
            levels.push_back(std::move(expression));
            return true;
        }        
        return false;        
    }

    bool addLevel(std::unique_ptr<ArrayLevel> arraylevel)
    {
        this->levels.merge(arraylevel->levels);
        return true;
    }
};

// Expressions Nodes

class StringExp : public Expression
{
    std::string value;
    int line, column;

public:
    StringExp(int line, int column, const std::string &value) : line(line), column(column), value(value) {}
};

class IntExp : public Expression
{
    int value;
    int line, column;

public:
    IntExp(int line, int column, int value) : line(line), column(column), value(value) {}
};

class DoubleExp : public Expression
{
    double value;
    int line, column;

public:
    DoubleExp(int line, int column, double value) : line(line), column(column), value(value) {}
};

class FloatExp : public Expression
{
    float value;
    int line, column;

public:
    FloatExp(int line, int column, float value) : line(line), column(column), value(value) {}
};

class BooleanExp : public Expression
{
    bool value;
    int line, column;

public:
    BooleanExp(int line, int column, bool value) : line(line), column(column), value(value) {}
};

class NullExp : public Expression
{
    int line, column;

public:
    NullExp(int line, int column) : line(line), column(column) {}
};

class VarExp : public Expression
{
    std::string name;
    int line, column;

public:
    VarExp(int line, int column, const std::string &name) : line(line), column(column), name(name) {}

    std::string getName()
    {
        return this->name;
    }
};

class AddExp : public Expression
{
    std::unique_ptr<Expression> left_expression, right_expression;
    int line, column;

public:
    AddExp(int line, int column,
                std::unique_ptr<Expression> left_expression,
                std::unique_ptr<Expression> right_expression) 
                : line(line), column(column),
                left_expression(std::move(left_expression)),
                right_expression(std::move(right_expression)) {}
};

class SubExp : public Expression
{
    std::unique_ptr<Expression> left_expression, right_expression;
    int line, column;

public:
    SubExp(int line, int column,
                        std::unique_ptr<Expression> left_expression,
                        std::unique_ptr<Expression> right_expression) : line(line), column(column),
                        left_expression(std::move(left_expression)),
                        right_expression(std::move(right_expression)) {}
};

class MulExp : public Expression
{
    std::unique_ptr<Expression> left_expression, right_expression;
    int line, column;

public:
    MulExp(int line, int column,
                             std::unique_ptr<Expression> left_expression,
                             std::unique_ptr<Expression> right_expression) 
                             : line(line), column(column),
                                left_expression(std::move(left_expression)),
                                right_expression(std::move(right_expression)) {}
};

class DivExp : public Expression
{
    std::unique_ptr<Expression> left_expression, right_expression;
    int line, column;

public:
    DivExp(int line, int column,
                       std::unique_ptr<Expression> left_expression,
                       std::unique_ptr<Expression> right_expression) 
                       : line(line), column(column), 
                        left_expression(std::move(left_expression)), 
                        right_expression(std::move(right_expression)) {}
};

class CallExpression : public Expression
{
    std::string id;    
    std::vector<std::unique_ptr<Expression>> actual_parameters;
    int line, column;

public:
    CallExpression(int line, int column, 
                   const std::string &id,std::vector<std::unique_ptr<Expression>> args) 
                   : line(line), column(column), id(id), 
                   actual_parameters(std::move(args)) {}
};

// /*************/

enum TYPE
{
    TINT,
    TSTRING,
    TDOUBLE,
    TFLOAT,
    TCHAR,
    TBOOL,
    TVOID,
    TCLASS
};

class Type
{
    TYPE type;
    std::string name;

public:
    Type(TYPE type) : type(type) {}
    Type(TYPE type, const std::string name): type(type), name(name){}
};
/*************/

class Parameter : public Instruction
{
    std::string name;
    std::unique_ptr<Type> type;
    int line, column;
    std::optional<std::unique_ptr<ArrayLevel>> levels; 

public:
    Parameter(int line, int column, std::unique_ptr<Type> type, std::string &name) 
        : line(line), column(column), type(std::move(type)), name(name) {}

    Parameter(int line, int column, std::unique_ptr<Type> type, std::string &name, std::unique_ptr<ArrayLevel> arraylevel) 
        : line(line), column(column), type(std::move(type)), name(name), levels(std::move(arraylevel)) {}
};

class FormalParameters : public Instruction
{
    // Type type;
    std::list<std::unique_ptr<Parameter>> parameters;

public:
    FormalParameters(){}

    FormalParameters(std::list<std::unique_ptr<Parameter>> parameters): parameters(std::move(parameters)){}

    //[[deprecated("Ya no es compatible, o quizá si :v")]] 
    bool addParameter(std::unique_ptr<Parameter> parameter)
    {
        this->parameters.push_back(std::move(parameter));
        return true;
    }
};

class Block : public Instruction
{
    int line, column;
    std::list<std::unique_ptr<Instruction>> instructions;

public:
    Block(int line, int column, std::list<std::unique_ptr<Instruction>> instructions) 
        : line(line), column(column), instructions(std::move(instructions)) {}

    Block(int line, int column) : line(line), column(column) {}

    bool addInstruction(std::unique_ptr<Instruction> instruction)
    {
        instructions.push_back(std::move(instruction));
        return true;
    }
};


class FunctionInst : public Instruction
{
    std::string name;
    //std::vector<std::unique_ptr<Parameter>> formalParameters;
    std::unique_ptr<FormalParameters> formalParameters;
    std::unique_ptr<Block> body;
    int line, column;

public:
    FunctionInst(int line, int column, const std::string &name,
                 std::unique_ptr<FormalParameters> formalParameters, std::unique_ptr<Block> body) 
                 : line(line), column(column), name(name), 
                    formalParameters(std::move(formalParameters)), body(std::move(body)) {}
};


class Program : public Instruction
{
    std::unique_ptr<Block> imports;
    std::unique_ptr<Block> globals;

public:
    Program(std::unique_ptr<Block> imports,
            std::unique_ptr<Block> globals) 
            : imports(std::move(imports)), 
            globals(std::move(globals)) {}    
};

class Import : public Instruction
{
    int line, column;
    std::string path;

public:
    Import(int line, int column, const std::string &path) : line(line), column(column), path(path) {}
};

class Constant : public Instruction
{
    int line, column;
    // Type type;
    std::string name;
    std::unique_ptr<Expression> value;

public:
    Constant(int line, int column,const std::string &name, std::unique_ptr<Expression> value) 
    : line(line), column(column), name(name), value(std::move(value)) {}
};

class Function : public Instruction
{
    int line, column;
    std::string name;
    // std::list<Parameter> formal_parameters;
    std::unique_ptr<FormalParameters> formal_parameters;
    std::unique_ptr<Block> block;
    Type *type;

public:
    Function(int line, int column, Type *type, const std::string &name, 
    std::unique_ptr<FormalParameters> parameters, std::unique_ptr<Block> block) 
    : line(line), column(column), type(type), name(name), 
      formal_parameters(std::move(parameters)), block(std::move(block)) {}
};

class Declaration : public Instruction
{
    int line, column;
    std::optional<std::list<std::string>> ids;
    std::optional<std::unique_ptr<Expression>> value;
    Type type;

public:
    Declaration(int line, int column, Type *type, std::list<std::string> ids, std::unique_ptr<Expression> value) 
    : line(line), column(column), ids(std::move(ids)), 
        value(std::move(value)), type(*type) {}

    Declaration(int line, int column, Type *type, std::list<std::string> ids) 
        : line(line), column(column), ids(std::move(ids)), type(*type) {}
};

class idList : public Instruction
{
    std::list<std::string> ids;
    int line, column;

public:
    idList(int line, int column) : line(line), column(column) {}
    idList(int line, int column, std::list<std::string> ids) : line(line), column(column), ids(std::move(ids)) {}

    bool addId(std::string id)
    {
        this->ids.push_back(id);
        return true;
    }    
};

class Assignation : public Instruction
{
    int line, column;
    std::string name;
    std::unique_ptr<Expression> expression;

public:
    Assignation(int line, int column, const std::string &name, 
        std::unique_ptr<Expression> expression) 
        : line(line), column(column), name(name), expression(std::move(expression)) {}
};

class For : public Instruction
{
    int line, column;
    std::unique_ptr<Instruction> declAssig;
    std::unique_ptr<Expression> condition;    
    std::unique_ptr<Expression> update;
    std::unique_ptr<Block> block;

public:
    For(int line, int column, std::unique_ptr<Instruction> decl, 
        std::unique_ptr<Expression> cond, std::unique_ptr<Expression> update, std::unique_ptr<Block> block) 
        : line(line), column(column), declAssig(std::move(decl)), condition(std::move(cond)), 
            update(std::move(update)), block(std::move(block)) {}
};

class DoWhile : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

public:
    DoWhile(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block) 
        : line(line), column(column), condition(std::move(condition)), block(std::move(block)) {}
};

class While : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

public:
    While(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block) 
        : line(line), column(column), condition(std::move(condition)), block(std::move(block)) {}
};

class IfInst : public Instruction
{
    int line, column;
    std::optional<std::unique_ptr<Expression>> condition;
    std::optional<std::unique_ptr<IfInst>> elseIf;
    std::unique_ptr<Block> block;

public:
    IfInst(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block) 
    : line(line), column(column), condition(std::move(condition)), block(std::move(block)) {}

    IfInst(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block, std::unique_ptr<IfInst> elseIf) 
    : line(line), column(column), 
        condition(std::move(condition)), block(std::move(block)), elseIf(std::move(elseIf)) {}
};

class ReturnInst: public Instruction{
    int line, column;
    std::unique_ptr<Expression> exp;

    public:
        ReturnInst(int line, int column, std::unique_ptr<Expression> exp)
            : line(line), column(column), exp(std::move(exp)){}
};

#endif