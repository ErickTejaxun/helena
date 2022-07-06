#include "CodeGen.h"
#include "parser.h"
#include "Semantic.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"


static llvm::cl::opt<std::string>
    Input(llvm::cl::Positional,
        llvm::cl::desc("<input expression>"),
        llvm::cl::init(""));

int main(int argc, const char **argv){
    llvm::InitLLVM X(argc, argv);
    llvm::cl::ParseCommandLineOptions(
        argc, argv, "Helena - Polyhedral compiler\n");

    Lexer Lex(Input);
    Parser Parser(Lex);
    AST *Tree = Parser.parse();
    if ( !Tree || Parser.hasError()){
        llvm::errs() << "Syntax errors occured\n";
        return 1;
    }

    Semantic semantic;
    if (semantic.semantic(Tree)){
        llvm::errs() << "Semantic errors occured\n";
        return 1;
    }

    CodeGen CodeGenerator;
    CodeGenerator.compile(Tree);
    return 0;
}