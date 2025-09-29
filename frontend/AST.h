#ifndef AST_H
#define AST_H

// #include "llvm/ADT/SmallVector.h"
// #include "llvm/ADT/StringRef.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Type.h"

#include <string>
#include <stdio.h>
#include <map>
#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <list>
#include <memory>
#include <optional>
#include <typeinfo> // Debug
#include <algorithm>
#include <stack>

class Error;
class ASTNode;

static std::unique_ptr<llvm::LLVMContext> Context;
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::Module> TheModule;
// static std::map<std::string, llvm::Value *> NamedValues;
class Type;
class HelenaVariable
{
private:
    Type *type;
    llvm::Value *Value;

public:
    HelenaVariable(llvm::Value *value, Type *type) : Value(value), type(type) {}
    Type *getType() { return type; }
    llvm::Value *getValue() { return Value; }
    void setType(Type *type) { type = type; }
    void setValue(llvm::Value *value) { Value = value; }
};
static std::map<std::string, HelenaVariable *> NamedValues;

enum TYPE
{
    TINT,
    TSTRING,
    TDOUBLE,
    TFLOAT,
    TCHAR,
    TBOOL,
    TVOID,
    TCLASS,
    TNULL
};

class Type
{
    TYPE type;
    std::string name;

public:
    Type() {}
    Type(TYPE type) : type(type) {}
    Type(TYPE type, const std::string name) : type(type), name(name) {}

    void setIntType()
    {
        type = TINT;
    }

    void setStringType()
    {
        type = TSTRING;
    }

    bool isNumeric()
    {
        return type == TINT || type == TDOUBLE || type == TCHAR;
    }

    llvm::Type *generateLLVMType(llvm::LLVMContext &context)
    {
        switch (type)
        {
            case TINT:
                return llvm::Type::getInt32Ty(context);
            case TDOUBLE:
                return llvm::Type::getDoubleTy(context);
            case TSTRING:
            {
                auto int8Ty = llvm::Type::getInt8Ty(context);
                return llvm::PointerType::get(int8Ty, 0);
            }
            case TBOOL:
                return llvm::Type::getInt1Ty(context);
            case TCHAR:
                return llvm::Type::getInt8Ty(context);
            case TVOID:
                return llvm::Type::getVoidTy(context);
            default:
                return nullptr;
                // On going implement classes types
        }
    }
};
/*************/

// std::unique_ptr<ASTNode> LogError(const char *Str) {
//   fprintf(stderr, "Error: %s\n", Str);
//   return nullptr;
// }

// llvm::Value *LogErrorV(const char *Str) {
//   LogError(Str);
//   return nullptr;
// }

static void InitializeModule()
{
    // Open a new context and module.
    Context = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("Helena", *Context);

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*Context);

    std::cout << "Module initialized" << std::endl;
}

/* Global configuration's variables*/
extern std::vector<Error> errorList;

// Utilidades para escribir
//  void writeModuleToFile(llvm::Module * TheModule, const std::string & filename){

//     std::error_code EC;

//     llvm::raw_fd_ostream outFile (filename, EC);

//     if(EC){
//         llvm::errs() << "Error abriendo archivo "<<filename << ": " << EC.message() << "\n";
//         return;
//     }

//     TheModule->print(outFile,nullptr);
// };

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
    std::string description;
    std::string type;
    std::string id;
    Error(int l, int c, std::string t, std::string i, std::string desc) : line(l), column(c), type(t), description(desc), id(i)
    {
        line = l;
        column = c;
    }

    std::string getMessage()
    {
        return "Error " + type + " in line: " + std::to_string(line) + ":\t" + description;
    }

    static void logSemanticError(int l, int c, std::string i, std::string d)
    {
        errorList.push_back(Error(l, c, std::string("Semantic"), i, d));
    }
};

// std::unique_ptr<ASTNode> LogError(const char *Str){
//     std::cerr<< "Error: " << Str<< std::endl;
//     return nullptr;
// }

// llvm::Value * LogErrorV(const char *Str){
//     LogError(Str);
//     return nullptr;
// }

static std::stack<llvm::BasicBlock*> LoopStack;


class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual llvm::Value *codegen() = 0;
};

class Expression : public ASTNode
{
public:
    virtual ~Expression() = default;
    llvm::Value *codegen() override = 0;
    virtual void print() {}
    virtual Type *getType() = 0;
};

class Instruction : public ASTNode
{
public:
    virtual ~Instruction() = default;
    llvm::Value *codegen() override = 0;
    virtual void print() {}
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
        if (expression)
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
    StringExp(int line, int column, const std::string &value) : line(line), column(column), value(value.substr(1, value.length() - 2)) {}

    llvm::Value *codegen() override
    {
        // True due the close char is like C
        // return llvm::ConstantDataArray::getString(*Context, value, true);
        return Builder.get()->CreateGlobalStringPtr(value);
    }

    Type *getType() override
    {
        // return new Type(TSTRING);
        Type *localType = new Type();
        localType->setIntType();
        return localType;
    }
};

class IntExp : public Expression
{
    int value;
    int line, column;

public:
    IntExp(int line, int column, int value) : line(line), column(column), value(value) {}

    llvm::Value *codegen() override
    {
        std::cout << "Integer expression node" << std::endl;
        return llvm::ConstantInt::get(*Context, llvm::APInt(32, value));
    }

    Type *getType() override
    {
        return new Type(TINT);
        // Type *localType = new Type();
        // localType->setIntType();
        // return localType;
    }

    int getValue() { return value; }
};

class DoubleExp : public Expression
{
    double value;
    int line, column;

public:
    DoubleExp(int line, int column, double value) : line(line), column(column), value(value) {}

    llvm::Value *codegen() override
    {
        return llvm::ConstantFP::get(*Context, llvm::APFloat(value));
    }

    Type *getType() override
    {
        return new Type(TDOUBLE);
    }
};

class FloatExp : public Expression
{
    float value;
    int line, column;

public:
    FloatExp(int line, int column, float value) : line(line), column(column), value(value) {}
    llvm::Value *codegen() override
    {
        return llvm::ConstantFP::get(*Context, llvm::APFloat(value));
    }

    Type *getType() override
    {
        return new Type(TFLOAT);
    }
};

class BooleanExp : public Expression
{
    bool value;
    int line, column;

public:
    BooleanExp(int line, int column, bool value) : line(line), column(column), value(value) {}

    llvm::Value *codegen() override
    {
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*Context),value);
    }

    Type *getType() override
    {
        return new Type(TBOOL);
    }
};

class NullExp : public Expression
{
    int line, column;

public:
    NullExp(int line, int column) : line(line), column(column) {}

    llvm::Value *codegen() override
    {
        return llvm::ConstantPointerNull::get(nullptr);
    }
    Type *getType() override
    {
        return new Type(TNULL);
    }
};

class VarArrayExp : public Expression
{
    std::string name;
    std::unique_ptr<Expression> exp;
    int line, column;

public:
    VarArrayExp(int line, int column, const std::string &name, std::unique_ptr<Expression> e) : line(line), column(column), name(name), exp(std::move(e)) {}

    std::string getName()
    {
        return this->name;
    }

    llvm::Value *codegen() override
    {
        std::cout << "Buscando variable array " << name << std::endl;
        // llvm::Value *V = NamedValues[name];
        HelenaVariable *V = NamedValues[name];
        if (!V)
        {
            // LogErrorV("Variable not exists in this enviroment.");
            std::cout << "No se ha encontrado la variable." << std::endl;
            for (auto var : NamedValues)
            {
                // std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>"<< var.second->getName().upper()<<std::endl;
                std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>" << var.first << std::endl;
            }
            return nullptr;
        }

        llvm::Value *indexV = exp.get()->codegen();
        if (!indexV)
        {
            std::cout << "invalid Expression :'v" << std::endl;
            return nullptr;
        }

        // Generamos la instruccion GetElementPtr GEP para obtener la dirección del elemento.

        llvm::Value *getInst = Builder->CreateGEP(V->getType()->generateLLVMType(*Context), V->getValue(), indexV, "array.element.ptr");
        // Builder->CreateLoad(V->getType()->generateLLVMType(*Context), V->getValue());

        // Generamos la instrucción load para obtener el valor de la dirección que se obtuvo en la instrucción anterior
        llvm::Value *loadInst = Builder->CreateLoad(getInst->getType(), getInst, "array.element.value");
        return loadInst;
    }

    Type *getType() override
    {
        std::cout << "Buscando tipo de variables array " << name << std::endl;
        // llvm::Value *V = NamedValues[name];
        HelenaVariable *V = NamedValues[name];
        if (!V)
        {
            // LogErrorV("Variable not exists in this enviroment.");
            std::cout << "No se ha encontrado la variable alv." << std::endl;
            for (auto var : NamedValues)
            {
                // std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>"<< var.second->getName().upper()<<std::endl;
                std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>" << var.first << std::endl;
            }
            return nullptr;
        }
        std::cout << "Variable " << name << " Tipo: " << std::endl;
        V->getType()->generateLLVMType(*Context)->print(llvm::errs());
        return V->getType();
    }
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

    llvm::Value *codegen() override
    {
        std::cout << "Buscando variable " << name << std::endl;
        HelenaVariable *V = NamedValues[name];
        try
        {
            std::cout << "Nombre variable " << V->getType() << std::endl;
            if (!V)
            {
                // LogErrorV("Variable not exists in this enviroment.");                
                for (auto var : NamedValues)
                {
                    // std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>"<< var.second->getName().upper()<<std::endl;
                    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>" << var.first << std::endl;
                }
                return nullptr;
            }
            // Aplicar verificacion de tipo de llamada si de referencia o por valor.

            //V->getType()->generateLLVMType(*Context)->print(llvm::errs());
            return Builder->CreateLoad(V->getType()->generateLLVMType(*Context), V->getValue());
            //Builder->CreateLoad(V->getType()->generateLLVMType(*Context),llvm::ConstantInt::get(*Context, llvm::APInt(32, 100)));
            
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error pisado." << e.what() << std::endl;
        }
        return nullptr;
    }

    Type *getType() override
    {
        std::cout << "Buscando tipo de variables " << name << std::endl;
        // llvm::Value *V = NamedValues[name];
        HelenaVariable *V = NamedValues[name];
        if (!V)
        {
            // LogErrorV("Variable not exists in this enviroment.");
            std::cout << "No se ha encontrado la variable alv." << std::endl;
            for (auto var : NamedValues)
            {
                // std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>"<< var.second->getName().upper()<<std::endl;
                std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>" << var.first << std::endl;
            }
            return nullptr;
        }
        std::cout << "Variable " << name << " Tipo: " << std::endl;
        V->getType()->generateLLVMType(*Context)->print(llvm::errs());
        return V->getType();
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

    llvm::Value *codegen() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();

        llvm::Value *l = left_expression.get()->codegen();
        llvm::Value *r = right_expression.get()->codegen();

        if (typeL->generateLLVMType(*Context) == typeR->generateLLVMType(*Context))
        {
            return Builder->CreateAdd(l, r);
        }
        else
        {

            if (l->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << l << std::endl;
                l = Builder->CreateLoad(r->getType(), l);
            }
            else
            {
                // l = Builder->CreateLoad(l->getType(),l);
            }

            if (r->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << r->getType()->isIntegerTy() << std::endl;
                r = Builder->CreateLoad(l->getType(), r);
            }
            else
            {
                // r = Builder->CreateLoad(r->getType(),r);
            }

            return Builder->CreateSub(l, r);
        }
    }

    Type *getType() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();
        return new Type(TINT);
    }
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

    llvm::Value *codegen() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();

        llvm::Value *l = left_expression.get()->codegen();
        llvm::Value *r = right_expression.get()->codegen();

        if (typeL->generateLLVMType(*Context) == typeR->generateLLVMType(*Context))
        {
            return Builder->CreateSub(l, r);
        }
        else
        {

            if (l->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << l << std::endl;
                l = Builder->CreateLoad(r->getType(), l);
            }
            else
            {
                // l = Builder->CreateLoad(l->getType(),l);
            }

            if (r->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << r->getType()->isIntegerTy() << std::endl;
                r = Builder->CreateLoad(l->getType(), r);
            }
            else
            {
                // r = Builder->CreateLoad(r->getType(),r);
            }

            return Builder->CreateSub(l, r);
        }
    }

    Type *getType() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();
        return new Type(TINT);
    }
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

    llvm::Value *codegen() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();

        llvm::Value *l = left_expression.get()->codegen();
        llvm::Value *r = right_expression.get()->codegen();

        if (typeL->generateLLVMType(*Context) == typeR->generateLLVMType(*Context))
        {
            return Builder->CreateMul(l, r);
        }
        else
        {

            if (l->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << l << std::endl;
                l = Builder->CreateLoad(r->getType(), l);
            }
            else
            {
                // l = Builder->CreateLoad(l->getType(),l);
            }

            if (r->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << r->getType()->isIntegerTy() << std::endl;
                r = Builder->CreateLoad(l->getType(), r);
            }
            else
            {
                // r = Builder->CreateLoad(r->getType(),r);
            }

            return Builder->CreateSub(l, r);
        }
    }

    Type *getType() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();
        return new Type(TINT);
    }
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

    llvm::Value *codegen() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();

        llvm::Value *l = left_expression.get()->codegen();
        llvm::Value *r = right_expression.get()->codegen();

        if (typeL->generateLLVMType(*Context) == typeR->generateLLVMType(*Context))
        {
            return Builder->CreateUDiv(l, r);
        }
        else
        {

            if (l->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << l << std::endl;
                l = Builder->CreateLoad(r->getType(), l);
            }
            else
            {
                // l = Builder->CreateLoad(l->getType(),l);
            }

            if (r->getType()->isPointerTy())
            {
                std::cout << "Variable type pointer" << std::endl;
                std::cout << r->getType()->isIntegerTy() << std::endl;
                r = Builder->CreateLoad(l->getType(), r);
            }
            else
            {
                // r = Builder->CreateLoad(r->getType(),r);
            }

            return Builder->CreateSub(l, r);
        }
    }

    Type *getType() override
    {
        Type *typeL = left_expression.get()->getType();
        Type *typeR = right_expression.get()->getType();
        return new Type(TINT);
    }
};

class CompExp : public Expression
{
    int line, column;
    std::unique_ptr<Expression> l;
    std::unique_ptr<Expression> r;
    int operation;

public:
    CompExp(int l, int c, std::unique_ptr<Expression> left, std::unique_ptr<Expression> r, int op)
        : line(l), column(c), l(std::move(left)), r(std::move(r)), operation(op)
    {
    }

    llvm::Value *codegen() override
    {
        std::cout << "Comp Expression AST**************************************" << std::endl;

        llvm::Value *lV = l.get()->codegen();
        Type *lrT = l.get()->getType();
        llvm::Type *lT = lrT->generateLLVMType(*Context);

        llvm::Value *rV = r.get()->codegen();
        Type *rrT = r.get()->getType();
        llvm::Type *rT = rrT->generateLLVMType(*Context);

        std::cout << "Valores obtenidos" << std::endl;
        if (lrT->isNumeric() && rrT->isNumeric())
        {
            switch (operation)
            {
            case 1:
                return Builder->CreateICmpSLT(lV, rV);
            case 2:
                return Builder->CreateICmpSLE(lV, rV);
            case 3:
                return Builder->CreateICmpSGT(lV, rV);
            case 4:
                return Builder->CreateICmpSGE(lV, rV);
            case 5:
                return Builder->CreateICmpEQ(lV, rV);
            default:
                nullptr;
            }
        }
        else
        {
            std::cout << "Error, values invalids" << std::endl;
            return nullptr;
        }

        return nullptr;
    }

    Type *getType() override
    {
        return new Type(TBOOL);
    }
};

class CallExpression : public Expression
{
    std::string id;
    std::vector<std::unique_ptr<Expression>> actual_parameters;
    int line, column;

public:
    CallExpression(int line, int column,
                   const std::string &id, std::vector<std::unique_ptr<Expression>> args)
        : line(line), column(column), id(id),
          actual_parameters(std::move(args)) {}

    llvm::Value *codegen() override;
};

class CallInstr : public Instruction
{
    std::string id;
    std::vector<std::unique_ptr<Expression>> actual_parameters;
    int line, column;

public:
    CallInstr(int line, int column,
              const std::string &id, std::vector<std::unique_ptr<Expression>> args)
        : line(line), column(column), id(id),
          actual_parameters(std::move(args)) {}

    CallInstr(int line, int column,
              const std::string &id)
        : line(line), column(column), id(id) {}

    llvm::Value *codegen() override
    {

        // Buscar la funcion en el módulo/contexto
        llvm::Function *CalleF = TheModule->getFunction(id);

        // Verificar si existe
        if (!CalleF)
        {
            std::cout << "Función no encontrada." << std::endl;
            return nullptr;
            // return LogErrorV("Función no encontrada.");
        }

        // Verificar la firma de la función con los parámetros.
        if (CalleF->arg_size() != actual_parameters.size())
        {
            std::cout << "Función no encontrada con los parámetros indicados." << std::endl;
            return nullptr;
            // return LogErrorV("Función no encontrada con los parámetros indicados.");
        }

        std::cout << "Funcion encontrada" << std::endl;

        std::vector<llvm::Value *> ArgsV;
        // for(unsigned i = 0, e=actual_parameters.size(); i != e; i++){
        //     ArgsV.push_back(actual_parameters[i].get()->codegen());
        //     if(!ArgsV.back()){
        //         return nullptr;
        //     }
        // }
        // return nullptr;
        return Builder->CreateCall(CalleF, ArgsV, "calltmp");
    }
};

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

    llvm::Value *codegen() override
    {
        return nullptr;
    }

    Type *getType() { return this->type.get(); }
};

class FormalParameters : public Instruction
{

public:
    // Type type;
    std::list<std::unique_ptr<Parameter>> parameters;
    FormalParameters() {}

    FormalParameters(std::list<std::unique_ptr<Parameter>> parameters) : parameters(std::move(parameters)) {}

    //[[deprecated("Ya no es compatible, o quizá si :v")]]
    bool addParameter(std::unique_ptr<Parameter> parameter)
    {
        this->parameters.push_back(std::move(parameter));
        return true;
    }

    llvm::Value *codegen() override
    {
        return nullptr;
    }

    int getParamsNumber()
    {
        return parameters.size();
    }
};

class Block : public Instruction
{
    int line, column;

public:
    std::list<std::unique_ptr<Instruction>> instructions;

    Block(int line, int column, std::list<std::unique_ptr<Instruction>> instructions)
        : line(line), column(column), instructions(std::move(instructions)) {}

    Block(int line, int column) : line(line), column(column) {}

    bool addInstruction(std::unique_ptr<Instruction> instruction)
    {
        // std::cout << instructions.size() << std::endl;
        instructions.push_back(std::move(instruction));
        return true;
    }

    llvm::Value *codegen() override
    {
        std::cout << "AST Node: " << typeid(this).name() << std::endl;

        std::size_t numberOfInstructions = instructions.size();

        llvm::Value *currentValue = nullptr;
        for (const auto &ptr : instructions)
        {
            std::cout << typeid(ptr.get()).name() << std::endl;
            currentValue = ptr.get()->codegen();
        }
        return currentValue;
    }
};

class FunctionInst : public Instruction
{
    std::string name;
    std::unique_ptr<Type> type;
    // std::vector<std::unique_ptr<Parameter>> formalParameters;
    std::unique_ptr<FormalParameters> formalParameters;
    std::unique_ptr<Block> body;
    int line, column;

public:
    FunctionInst(int line, int column, const std::string &name,
                 std::unique_ptr<FormalParameters> formalParameters, std::unique_ptr<Block> body)
        : line(line), column(column), name(name),
          formalParameters(std::move(formalParameters)), body(std::move(body)) {}

    FunctionInst(int line, int column, std::unique_ptr<Type> type, const std::string &name,
                 std::unique_ptr<FormalParameters> formalParameters, std::unique_ptr<Block> body)
        : line(line), column(column), name(name), type(std::move(type)),
          formalParameters(std::move(formalParameters)), body(std::move(body)) {}

    llvm::Value *codegen() override
    {
        std::vector<llvm::Type *> functionType(formalParameters.get()->getParamsNumber());
        for (auto &fp : formalParameters.get()->parameters)
        {
            functionType.push_back(fp.get()->getType()->generateLLVMType(*Context));
        }

        std::cout << "Vector de tipos generado" << std::endl;
        llvm::FunctionType *FT =
            llvm::FunctionType::get(type.get()->generateLLVMType(*Context), functionType, false);

        llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());

        unsigned Idx = 0;
        for (auto &Arg : F->args())
        {
            Arg.setName(this->name);
        }

        // if (!body.get())
        // {
        //     std::cerr << "There is no instructions for this program." << std::endl;
        //     return nullptr;
        // }
        // return F;

        // Pendiente, agregar validación de que ya exista la función.

        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*Context, "entry", F);
        Builder->SetInsertPoint(BB);

        NamedValues.clear();
        for (llvm::Argument &Arg : F->args())
        {
            HelenaVariable *Variable = new HelenaVariable(&Arg, type.get());
            // Variable->setValue (&Arg);
            // Variable->setType(type.get());
            NamedValues[std::string(Arg.getName())] = Variable;
        }

        if (llvm::Value *RetVal = this->body->codegen())
        {
            // Builder->CreateRet(RetVal); //Se implenta el nodo retorno
            Builder->ClearInsertionPoint();
            // Se verifica la consistencia del código de la función generada.
            llvm::verifyFunction(*F);
        }

        return F;
    }
};

class Program : public Instruction
{
    std::unique_ptr<Block> imports;
    std::unique_ptr<Block> globals;

public:
    std::string name;
    Program(std::unique_ptr<Block> imports,
            std::unique_ptr<Block> globals)
        : imports(std::move(imports)),
          globals(std::move(globals)) {}

    Program(std::unique_ptr<Block> globals) : globals(std::move(globals)) {}

    void addMainCallInstruction()
    {
        std::unique_ptr<CallInstr> call = std::make_unique<CallInstr>(0, 0, "main");
        auto instCall = std::unique_ptr<Instruction>(call.release());
        // globals.get()->addInstruction(std::move(instCall));
    }

    void addBuildInFunctions()
    {

        llvm::Type *Int32Ty = Builder->getInt32Ty();
        llvm::Type *PtrTy = Builder->getPtrTy();

        llvm::FunctionType *PrintfFuncTy = llvm::FunctionType::get(
            Int32Ty,
            {PtrTy},
            true);

        llvm::FunctionCallee PrintfFunc = TheModule->getOrInsertFunction("printf", PrintfFuncTy);
    }

    llvm::Value *codegen() override
    {
        InitializeModule();
        addMainCallInstruction();
        addBuildInFunctions();
        if (!globals.get())
        {
            std::cerr << "There is no instructions for this program." << std::endl;
            return nullptr;
        }
        globals.get()->codegen();

        /*
        if(!Context){
            std::cerr << "Error, LLVMContext has not been initialized"<< std::endl;
            InitializeModule();
        }
        return llvm::ConstantFP::get(*Context, llvm::APFloat(6.66));
        */
        std::cout << "------------------------------------------" << std::endl
                  << std::endl;
        // TheModule->print(llvm::errs(), nullptr);
        name = name.replace(name.length() - 4, name.length(), "");
        const std::string &filename = name + ".ll";
        std::error_code EC;
        llvm::raw_fd_ostream outFile(filename, EC);

        if (EC)
        {
            llvm::errs() << "Error abriendo archivo " << filename << ": " << EC.message() << "\n";
            return nullptr;
        }

        TheModule->print(outFile, nullptr);

        return nullptr;
    }
};

class PrintInstr : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> exp;

public:
    PrintInstr(int line, int column, std::unique_ptr<Expression> exp) : line(line), column(column), exp(std::move(exp)) {}

    llvm::Value *codegen() override
    {
        llvm::Value *value = exp.get()->codegen();
        llvm::Type *valueType = exp.get()->getType()->generateLLVMType(*Context);
        llvm::Value *formatString = nullptr;
        llvm::Function *CalleF = TheModule->getFunction("printf");
        if (!CalleF)
        {
            // Significa que no existe la función printf en nuestro contexto/ambiente
            // If the functions doesn't exist, we will register it once, and only one time.
            llvm::FunctionType *printfType = llvm::FunctionType::get(Builder.get()->getInt32Ty(), {Builder.get()->getPtrTy()}, true);
            CalleF = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", *TheModule);
            CalleF->setCallingConv(llvm::CallingConv::C);
        }
        std::vector<llvm::Value *> ArgsV;
        // ArgsV.push_back(value);

        // Type semantics
        if (valueType->isIntegerTy())
        {
            formatString = Builder.get()->CreateGlobalStringPtr("%d\n");
            ArgsV.push_back(formatString);

            // Bits Size
            if (valueType->getIntegerBitWidth() < 32)
            {
                // We will extend the type value if the number is larger than 32 bits
                value = Builder.get()->CreateSExt(value, Builder.get()->getInt32Ty(), "print_sext_int");
            }
            else if (valueType->getIntegerBitWidth() > 32 && valueType->getIntegerBitWidth() < 64)
            {
                formatString = Builder.get()->CreateGlobalStringPtr("%ld\n");
                ArgsV[0] = formatString; // We replace the before type
                value = Builder.get()->CreateSExt(value, Builder.get()->getInt64Ty(), "print_sext_long");
            }
            else if (valueType->getIntegerBitWidth() == 64)
            {
                formatString = Builder.get()->CreateGlobalStringPtr("%ld\n");
                ArgsV[0] = formatString; // We replace the before type
            }
            ArgsV.push_back(value);
        }
        else if (valueType->isDoubleTy() || valueType->isFloatTy())
        {
            formatString = Builder.get()->CreateGlobalStringPtr("%f\n");
            ArgsV.push_back(formatString);

            if (valueType->isFloatTy())
            {
                value = Builder.get()->CreateFPExt(value, Builder.get()->getFloatTy(), "print_ext_fp");
            }
            ArgsV.push_back(value);
        }
        else if (valueType->isPointerTy()) // If is an opaque pointer
        {
            // We do not know what kind of type is this type of pointer (really yes, because we are implementing ours type system ;))
            formatString = Builder.get()->CreateGlobalStringPtr("%s\n");
            ArgsV.push_back(formatString);
            ArgsV.push_back(value);
        }
        else
        {
            std::cout << "Error perros" << std::endl;
            return nullptr;
        }

        if (formatString)
        {
            return Builder->CreateCall(CalleF, ArgsV, "calltmp");
        }
        return nullptr;
    }
};

class Import : public Instruction
{
    int line, column;
    std::string path;

public:
    Import(int line, int column, const std::string &path) : line(line), column(column), path(path) {}

    llvm::Value *codegen() override;
};

class Constant : public Instruction
{
    int line, column;
    // Type type;
    std::string name;
    std::unique_ptr<Expression> value;

public:
    Constant(int line, int column, const std::string &name, std::unique_ptr<Expression> value)
        : line(line), column(column), name(name), value(std::move(value)) {}

    llvm::Value *codegen() override;
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

    llvm::Value *codegen() override
    {
        if (!block.get())
        {
            std::cerr << "There is no instructions for this program." << std::endl;
            return nullptr;
        }
        block.get()->codegen();

        /*
        if(!Context){
            std::cerr << "Error, LLVMContext has not been initialized"<< std::endl;
            InitializeModule();
        }
        return llvm::ConstantFP::get(*Context, llvm::APFloat(6.66));
        */
        return nullptr;
    }
};

class Declaration : public Instruction
{
    int line, column;
    // std::optional<std::list<std::string>> ids;
    // std::optional<std::unique_ptr<Expression>> value;
    std::list<std::string> ids;
    std::unique_ptr<Expression> value;
    std::unique_ptr<Type> type;

public:
    Declaration(int line, int column, std::unique_ptr<Type> type, std::list<std::string> ids, std::unique_ptr<Expression> value)
        : line(line), column(column), ids(std::move(ids)),
          value(std::move(value)), type(std::move(type)) {}

    Declaration(int line, int column, std::unique_ptr<Type> type, const std::string &id, std::unique_ptr<Expression> value)
        : line(line), column(column), value(std::move(value)), type(std::move(type))
    {
        ids.push_front(id);
    }

    Declaration(int line, int column, std::unique_ptr<Type> type, std::list<std::string> ids)
        : line(line), column(column), ids(std::move(ids)), type(std::move(type)) {}

    llvm::Value *codegen() override
    {
        std::cout << "Declaration Node" << std::endl;
        std::vector<llvm::Value *> variables;

        for (const std::string &id : ids)
        {
            llvm::AllocaInst *allocation = Builder->CreateAlloca(type.get()->generateLLVMType(*Context), 0, id);
            HelenaVariable *Variable = new HelenaVariable(allocation, type.get());
            // Variable->setValue (allocation);
            // Variable->setType(type.get());
            variables.push_back(allocation);
            NamedValues[id] = Variable;
        }

        if (value != nullptr)
        {
            llvm::Value *val = value.get()->codegen();
            for (auto var : variables)
            {
                Builder->CreateStore(val, var);
            }
        }

        return nullptr; // Placeholder
    }
};

class DeclarationA : public Instruction
{
    std::string id;
    int line, column;
    std::unique_ptr<Expression> valueE;
    std::unique_ptr<Type> type;
    std::unique_ptr<Type> typeE;

public:
    DeclarationA(int l, int c, std::unique_ptr<Type> type, const std::string &id, std::unique_ptr<Type> typeE, std::unique_ptr<Expression> exp) : line(l), column(c), type(std::move(type)), id(std::move(id)), typeE(std::move(typeE)), valueE(std::move(exp)) {};

    llvm::Value *codegen() override
    {
        std::cout << "Array declaration " << std::endl;

        llvm::Value *expV = valueE.get()->codegen();
        llvm::Type *expType = valueE.get()->getType()->generateLLVMType(*Context);
        llvm::Type *ArrayType = type.get()->generateLLVMType(*Context);
        if (ArrayType == expType)
        {
            if (IntExp *actualExpression = dynamic_cast<IntExp *>(valueE.get()))
            {
                llvm::ArrayType *newType = llvm::ArrayType::get(ArrayType, actualExpression->getValue());

                llvm::AllocaInst *allocation = Builder->CreateAlloca(newType, nullptr, id);
                HelenaVariable *Variable = new HelenaVariable(allocation, type.get());
                NamedValues[id] = Variable;
                return allocation;
            }
            else
            {
                llvm::ArrayType *newType = llvm::ArrayType::get(ArrayType, 0);
            }
            // newType->setArraySize(llvm::ConstantInt::get(valueE.get()->getType(),valueE));
        }
        std::cout << "Error, los tipos no coinciden." << std::endl;
        return expV; // Placeholder
    }
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

    llvm::Value *codegen() override;
};

class Assignment : public Instruction
{
    int line, column;
    std::string name;
    std::unique_ptr<Expression> expression;

public:
    Assignment(int line, int column, const std::string &name,
               std::unique_ptr<Expression> expression)
        : line(line), column(column), name(name), expression(std::move(expression)) {}

    llvm::Value *codegen() override
    {
        std::cout << "Assignment Node" << std::endl;
        return nullptr; // Placeholder
    }
};

class ArrayAssignment : public Instruction
{
    int line, column;
    std::string name;
    std::unique_ptr<Expression> indexExp;
    std::unique_ptr<Expression> exp;

public:
    ArrayAssignment(int line, int column, const std::string &name,
                    std::unique_ptr<Expression> iExp,
                    std::unique_ptr<Expression> exp)
        : line(line), column(column), name(name), indexExp(std::move(iExp)), exp(std::move(exp)) {}

    llvm::Value *codegen() override
    {
        std::cout << "Buscando variable array " << name << std::endl;
        // llvm::Value *V = NamedValues[name];
        HelenaVariable *V = NamedValues[name];
        if (!V)
        {
            // LogErrorV("Variable not exists in this enviroment.");
            std::cout << "No se ha encontrado la variable alv." << std::endl;
            for (auto var : NamedValues)
            {
                // std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>"<< var.second->getName().upper()<<std::endl;
                std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>" << var.first << std::endl;
            }
            return nullptr;
        }

        // Validamos el índice
        llvm::Value *indexV = indexExp.get()->codegen();
        if (!indexV)
        {
            std::cout << "invalid Expression :'v" << std::endl;
            return nullptr;
        }

        // Validamos el nuevo valor
        //  ¿Dónde valimos los tipos? PTM
        llvm::Value *newValue = exp.get()->codegen();
        if (!newValue)
        {
            std::cout << "Invalid new value" << std::endl;
            return nullptr;
        }

        // Generamos la instruccion GetElementPtr GEP para obtener la dirección del elemento.
        llvm::Value *getInst = Builder->CreateGEP(V->getType()->generateLLVMType(*Context), V->getValue(), indexV, "array.element.ptr");

        // Generamos la instrucción store para guardar el valor nuevo.
        return Builder->CreateStore(newValue, getInst);
    }
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

    llvm::Value *codegen() override;
};

class DoWhile : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

public:
    DoWhile(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block)
        : line(line), column(column), condition(std::move(condition)), block(std::move(block)) {}

    llvm::Value *codegen() override
    {
        llvm::Function *parent = Builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(*Context, "do.body", parent);
        llvm::BasicBlock *headerBlock = llvm::BasicBlock::Create(*Context, "do.cond");
        llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(*Context, "do.end");

        // Creamos un salto hacia el nuevo bloque de código
        Builder->CreateBr(bodyBlock);

        // Set the enter pointer for body loop (si no, no sabe dónde empezar.)
        Builder->SetInsertPoint(bodyBlock);

        // Generamos el código del body del ciclo
        block.get()->codegen();

        // Cambiamos de bloque
        Builder->CreateBr(headerBlock);
        Builder->SetInsertPoint(headerBlock);

        llvm::Value *conditionValue = condition.get()->codegen();
        if (conditionValue->getType() != Builder->getInt1Ty())
        {
            conditionValue = Builder->CreateICmpNE(conditionValue, Builder->getInt32(0), "loop.cond");
        }

        Builder->CreateCondBr(conditionValue, bodyBlock, endBlock);

        // parent->getBasicBlockList().push_back(endBlock);
        endBlock->insertInto(parent);
        Builder->SetInsertPoint(endBlock);
        return nullptr;
    }
};

class While : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> block;

public:
    While(int line, int column, std::unique_ptr<Expression> condition, std::unique_ptr<Block> block)
        : line(line), column(column), condition(std::move(condition)), block(std::move(block)) {}

    llvm::Value *codegen() override
    {
        
        std::cout << "While generation" << std::endl;
        
        llvm::Function *parent = Builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *headerBlock = llvm::BasicBlock::Create(*Context, "while.header",parent);
        llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(*Context, "while.body",parent);
        llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(*Context, "while.exit",parent);

        LoopStack.push(endBlock);
        // Creamos un salto hacia el bloque cabecera
        Builder->CreateBr(headerBlock);

        // Set the enter pointer for body loop (si no, no sabe dónde empezar.)
        Builder->SetInsertPoint(headerBlock);

        llvm::Value *conditionValue = condition.get()->codegen();
        llvm::Type *boolTy = llvm::Type::getInt1Ty(*Context);
        llvm::Value *isTrue = Builder->CreateICmpNE(conditionValue, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*Context),0),"while.cond");
        Builder->CreateCondBr(isTrue, bodyBlock, endBlock);
        

        Builder->SetInsertPoint(bodyBlock);
        block.get()->codegen();

        LoopStack.pop();
        Builder->CreateBr(headerBlock);
        return nullptr;
    }
};


class BreakInst: public Instruction{
    int line, column;
    
    public:
        BreakInst(int l, int c): line(l),column(c){};


        llvm::Value * codegen() override{
            if(LoopStack.size()==0){
                throw std::runtime_error("Error en la pila de ciclos");
            }

            llvm::BasicBlock* exitBlock = LoopStack.top();
            LoopStack.pop();
            Builder->CreateBr(exitBlock);
            return nullptr;

        }
};

class ContinueInst: public Instruction{
    int line, column;
    
    public:
        ContinueInst(int l, int c): line(l),column(c){};

        llvm::Value * codegen() override{
            if(LoopStack.size()==0){
                throw std::runtime_error("Error en la pila de ciclos");
            }

            llvm::BasicBlock* exitBlock = LoopStack.top();
            LoopStack.pop();
            Builder->CreateBr(exitBlock);
            return nullptr;

        }
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

    llvm::Value *codegen() override;
};

class ReturnInst : public Instruction
{
    int line, column;
    std::unique_ptr<Expression> exp;

public:
    ReturnInst(int line, int column, std::unique_ptr<Expression> exp)
        : line(line), column(column), exp(std::move(exp)) {}

    llvm::Value *codegen() override
    {
        // Tenemos que tener la funcion actual para saber el tipo.
        llvm::Function *currentFunction = Builder->GetInsertBlock()->getParent();

        llvm::Value *ret = exp.get()->codegen();
        if (ret->getType()->isPointerTy())
        {
            llvm::Type *loadedType = ret->getType();

            // Verificamos que coincidan los tipos
            if (currentFunction->getReturnType() != loadedType)
            {
                // Imprimos error
            }

            return Builder->CreateRet(Builder->CreateLoad(currentFunction->getReturnType(), ret, "loadtmp"));
        }

        // Agregar casteos explícitos y otras validaciones

        return Builder->CreateRet(ret);
        // return exp.get()->codegen();
    }
};

#endif