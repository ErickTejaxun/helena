#fichero Makefile

OBJ = parser.o scanner.o 

helena : $(OBJ)     #segunda fase de la tradicción. Generación del código ejecutable 
	g++ -o helena $(OBJ)

parser.o : parser.c        #primera fase de la traducción del analizador sintáctico
	g++ -c -o parser.o  parser.c 
	
scanner.o : lex.yy.c		#primera fase de la traducción del analizador léxico
	g++ -c -o scanner.o  lex.yy.c 	

parser.c : parser.y       #obtenemos el analizador sintáctico en C
	bison -d   -v parser.y -o parser.c

lex.yy.c: scanner.l	#obtenemos el analizador léxico en C
	flex scanner.l


test: 
	./helena

clean : 
	rm  -f  *.c *.o *.output 