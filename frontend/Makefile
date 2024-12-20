# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.
MLIR_INCLUDE_PATH = /usr/include/llvm-18#/home/erick/llvm-project/mlir/include/
LLVM_INCLUDE_PATH = /usr/include/llvm-18#/home/erick/llvm-project/build/include/
LLVM_CURRENT_pATH = /usr/include/llvm-18

BASE = helen-frontend++
BISON = bison
CXX = clang++-18#g++
FLEX = flex

all: $(BASE)

%.cc %.hh %.html %.gv: %.yy
	$(BISON) $(BISONFLAGS) --html --graph -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -I$(MLIR_INCLUDE_PATH) -I$(LLVM_INCLUDE_PATH) -c -o$@ $< 

$(BASE): $(BASE).o driver.o parser.o scanner.o
	$(CXX) -o $@ $^

$(BASE).o: parser.hh
parser.o: parser.hh
scanner.o: parser.hh

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$< -

CLEANFILES =										\
  $(BASE) *.o										\
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh	\
  scanner.cc
clean:
	rm -f $(CLEANFILES)
