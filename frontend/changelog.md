## 09022026
- Se ha ajustado. Había una CreateLoad de más y también se estaba creando el segundo (que quedó como único) CreateLoad con argumentos incorrectos: se estaba usando getInst->getType que daba una dirección de 8 bytes en 
vez del tipo de variable (en las pruebas i32, por lo tanto 4 bytes).
- ./helen-frontend++ example.cpp ; clang example.ll -o ejecutable; ./ejecutable
- Comenzando a trabajar para cambio de entornos. 
- Se está trabajando eso en el archivo example.cpp

## 01012026
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


## 30092025

- Ajustar el manejo de BB.
- Se ajustó el while y ya funciona con operaciones básicas. 
- Se tiene que validar por qué cuando hay asignaciones no funciona correctamente (bueno asignaciones y prints) los whiles.
- Se comenzó a implementar if else, se agregó a la gramática y AST pero mañana implementamos la lógica para generar LLVM IR y MLIR.

## 26102025
- Se ajustó el código del scanner para generar las operaciones de comparación, estaba al revés y por eso no generaba correctamente el gt y slt.
- Ya funciona bien los bucles. 
- Ahora tenemos un error en los tipos cuando se hacen operaciones entre primitivos y elementos de arreglos. Mañana lo arreglamos.

## 16042026
- Se agrega soporte para macos (en el makefile). 
- Se agrega flags para evitar errores en carga debido a ruta de librerías compartidas no encontradas, se agrega flags para el linker.

## 16042026 
- Se valida que el soporte para operaciones entre vectores está funcionando correctamente (al menos para las sumas).

## 17042026
- Se valida que el fix para soportar operaciones entre arreglos con punteros opacos funciona para operaciones binarias. 
- Se valida que se ha hecho cambio para utilizar punteros opacos para soportar llvm 22. 
- Se realiza un ajuste en el retorno del tipo cadena. Esto soluciona el problema de la impresión directa de cadenas. print("hola");