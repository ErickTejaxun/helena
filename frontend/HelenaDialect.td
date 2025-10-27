include "mlir/IR/OpBase.td"
include "mlir/Interfaces/FunctionInterfaces.td"
include "mlir/IR/SymbolInterfaces.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

def Helena_Dialect : Dialect  {

    let name = "helena";

    let summary = "A high-level dialect for analyzing and optimizing the Helena Lenguage by ErickTejaxun";

    let description = [{
        Este es mi trabajo de fin de grado, inicio de trabajo de fin de master y un proyecto para poder investigar 
        temas de optimización, paralelización automática y arquitectura de computadoras. 
        Este será el inicio de una nueva compañía dedicada a estos temas. 
    }];

    let cppNamespace = "helena";

}