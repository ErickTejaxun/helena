#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "Helena/Basic/Version.h"


int main(int argc_, const char **argv_){
    llvm::InitLLVM X(argc_, argv_);
    llvm::outs() << "Welcome to Helena "
                 << helena::getHelenaVersion()
                 << "\n";

}