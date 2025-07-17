#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h" // Para imprimir el IR

int main() {
    llvm::LLVMContext TheContext;
    std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("my_module", TheContext);
    llvm::IRBuilder<> Builder(TheContext);

    // 1. Declarar printf
    llvm::Type* Int8PtrTy = Builder.getInt8PtrTy();
    llvm::Type* Int32Ty = Builder.getInt32Ty();

    llvm::FunctionType* PrintfFuncTy = llvm::FunctionType::get(
        Int32Ty,
        {Int8PtrTy},
        true
    );
    llvm::FunctionCallee PrintfFunc = TheModule->getOrInsertFunction("printf", PrintfFuncTy);

    // 2. Crear la cadena de formato "Hello, LLVM!\n"
    llvm::StringRef FormatStr = "Hello, LLVM!\n";
    llvm::Constant* FormatConstant = llvm::ConstantDataArray::getString(TheContext, FormatStr);
    llvm::ArrayType* FormatArrayType = llvm::ArrayType::get(Builder.getInt8Ty(), FormatStr.size() + 1);

    llvm::GlobalVariable* FormatGlobal = new llvm::GlobalVariable(
        *TheModule,
        FormatArrayType,
        true,
        llvm::GlobalValue::PrivateLinkage,
        FormatConstant,
        "format_str"
    );

    // Obtener un puntero a la cadena de formato
    llvm::Value* FormatPtr = Builder.CreateConstGEP2_32(
        FormatGlobal->getValueType(),
        FormatGlobal,
        0, 0,
        "format_ptr"
    );

    // 3. Crear la función main
    llvm::Function* MainFunc = llvm::Function::Create(
        llvm::FunctionType::get(Builder.getInt32Ty(), false), // int main()
        llvm::Function::ExternalLinkage,
        "main",
        TheModule.get()
    );
    llvm::BasicBlock* EntryBlock = llvm::BasicBlock::Create(TheContext, "entry", MainFunc);
    Builder.SetInsertPoint(EntryBlock);

    // 4. Llamar a printf
    Builder.CreateCall(PrintfFunc, {FormatPtr}, "call_printf");

    // 5. Retornar 0 desde main
    Builder.CreateRet(Builder.getInt32(0));

    // Imprimir el LLVM IR generado
    TheModule->print(llvm::outs(), nullptr);

    return 0;
}