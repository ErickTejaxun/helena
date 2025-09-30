## 01012025
- Fixed the path to libs created when builded LLVM. ./build/lib
- Fixed the linked phase indicated to lld that we are going to use -lLLVMSupport to use EnableABI.
- Fixed the differents implementation of the key functions of Instruction and Expression definition.

## 082025
- No recordaba que llvm no tiene una tabla global y uno tiene que seguir mantenerlo. 
- Se implementó soporte para arreglos de tamaño fijo. Siguiente paso es crear para tamaños calculados en tiempo de ejecución (habrá que trabajar con los punteros de llvm.) Mañana trabajamos en eso.

## 28092025
- Adding comments support in our lexer. Now we can use /* */ to comment multiple lines. Usando multiples estados.

## 29092025
- Adding comments support in our lexer. Now we can use // to comment ines. Using flex states.
- Se implementó parcialmente la asignación ya que está asignado el valor a otro espacio en la memoria. 
- Se ajustó los bloques para el ciclo while ya que estaban en orden incorrecto. 