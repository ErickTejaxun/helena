#include "CodeGen.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace{
    class ToIRVisitor: public ASTVisitor{
        Module *M;
        IRBuilder<> Builder;
        Type *VoidTy;
        Type *Int32Ty;
        Type *Int8PtrTy;
        Type *Int8PtrPtrTy;
        Constant *Int32Zero;
        Value *V;
        StringMap<Value *> nameMap;

        public:
            ToIRVisitor(Module *M) : M(M), Builder(M->getContext()){
                VoidTy = Type::getVoidTy(M->getContext());
                Int32Ty = Type::getInt32Ty(M->getContext());
                Int8PtrTy = Type::getInt8PtrTy(M->getContext());
                Int8PtrPtrTy = Int8PtrTy->getPointerTo();
                Int32Zero = ConstantInt::get(Int32Ty, 0, true);
            }

            void run(AST *Tree){
                FunctionType *MainFty = FunctionType::get(
                    Int32Ty, {Int32Ty, Int8PtrPtrTy}, false);
                Function *MainFn = Function::Create(
                    MainFty, GlobalValue::ExternalLinkage, "main" , M);

                BasicBlock *BB = BasicBlock::Create(M->getContext(), "entry", MainFn);
                Builder.SetInsertPoint(BB);

                Tree->accept(*this);

                FunctionType *CalcWriteFnTy = 
                    FunctionType::get(VoidTy, {Int32Ty}, false);
                FunctionType *CalcWriteFn = Function::Create(
                    CalcWriteFnTy, GlobalValue::ExternalLinkage, "calc_write", M);
                
                Builder.CreateCall(CalcWriteFnTy, CalcWriteFn, {V});
                Builder.CreateRet(Int32Zero);

            }
    }
}