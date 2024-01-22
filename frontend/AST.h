#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Expr;
class Factor; 
class NumberLiteral;
class BinaryOp;
class Decl;
class Assig;
class Print;

class ASTVisitor
{
    public:
        virtual void visit(AST &){};
        virtual void visit(Expr &) {}; 
        virtual void visit(BinaryOp &) = 0;
        virtual void visit(Factor &) =0;
        virtual void visit(Decl &) = 0;
        virtual void visit(Assig &) = 0;
        virtual void visit(Print &) = 0;
};

class AST
{
    public:
        virtual ~AST()  {} 
        virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST 
{
    public:
        Expr() {}
};

class Factor : public Expr 
{
    public: 
        enum ValueKind {Ident, Number};
    
    private: 
        ValueKind Kind;
        llvm::StringRef Val;

    public: 
        Factor(ValueKind Kind, llvm::StringRef Val)
            : Kind(Kind), Val(Val) {} 
        
        ValueKind getKind() { return Kind;}

        llvm::StringRef getVal() { return Val;}

        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
};

class NumberLiteral : public Expr 
{
    public: 
        enum ValueKind {Double,Integer};
    
    private: 
        ValueKind Kind; 
        float Val;
    
    float getVal() { return Val;}

    virtual void accept(ASTVisitor &V) override {
        V.visit(*this);
    }
};

class BinaryOp : public Expr 
{
    public:
        enum Operator {Plus, Minus, Mul, Div}; 
    
    private: 
        Expr *Left;
        Expr *Right; 
        Operator Op;
    
    public: 
        BinaryOp(Operator Op, Expr *L, Expr *R)
            :Op(Op), Left(L), Right(R){};
        
        Expr *getLeft(){ return Left; }
        Expr *getRight(){ return Right; }

        Operator getOperator(){ return Op;}

        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
};

class Relational : public Expr {
    public:
        enum Operator {Lower, Greater, LowerEqual, GreaterEqual, Equal, Differnt}; 
    
    private: 
        Expr *Left;
        Expr *Right; 
        Operator Op;
    
    public: 
        BinaryOp(Operator Op, Expr *L, Expr *R)
            :Op(Op), Left(L), Right(R){};
        
        Expr *getLeft(){ return Left; }
        Expr *getRight(){ return Right; }

        Operator getOperator(){ return Op;}

        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }  
};

class Logical : public Expr {
    public:
        enum Operator {And, Or, Not}; 
    
    private: 
        Expr *Left;
        Expr *Right; 
        Operator Op;
    
    public: 
        BinaryOp(Operator Op, Expr *L, Expr *R)
            :Op(Op), Left(L), Right(R){};
        
        Expr *getLeft(){ return Left; }
        Expr *getRight(){ return Right; }

        Operator getOperator(){ return Op;}

        virtual void accept(ASTVisitor &V) override {
            V.visit(*this);
        }
};


class Decl : public AST {
    using VarVector = 
        llvm::SmallVector<llvm::StringRef , 8>;
    VarVector Vars;
    Expr *E;

    public: 
        Decl(llvm::SmallVector<llvm::StringRef, 8> Vars, 
            Expr *E)
            : Vars(Vars), E(E) {}
        
        VarVector::const_iterator begin(){ return Vars.begin(); }

        VarVector::const_iterator end() { return Vars.end(); }

        Expr *getExpr() { return E; }

        virtual void accept(ASTVisitor &V) override{
            V.visit(*this);
        }
};


#endif