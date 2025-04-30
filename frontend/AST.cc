#include "AST.h"

std::unique_ptr<ASTNode> LogError(const char *Str){
    std::cerr<< "Error: " << Str<< std::endl;
    return nullptr;
}

llvm::Value * LogErrorV(const char *Str){
    LogError(Str);
    return nullptr;
}