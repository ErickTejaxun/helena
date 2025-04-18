# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.
# /usr/bin/clang++-18
MLIR_INCLUDE_PATH = /home/erick/llvm-project/llvm/include/#/usr/include/llvm-18#/home/erick/llvm-project/mlir/include/
LLVM_INCLUDE_PATH = /home/erick/llvm-project/llvm/include/#/usr/include/llvm-18#/home/erick/llvm-project/build/include/
LLVM_C_INCLUDE_PATH = /home/erick/llvm-project/build4/include/
LLVM_CURRENT_PATH = /home/erick/llvm-project/llvm/include/#/usr/include/llvm-18

BASE= helen-frontend++
BISON=bison
CXX=clang++ #clang++-18#g++
FLEX=flex
#BISONFLAGS=$(llvm-config --cxxflags) 
#CXXFLAGS=$(llvm-config --cxxflags)
#-fno-exceptions
#$(llvm-config --ldflags) $(llvm-config --libs)
#CXXFLAGS = -L/home/erick/llvm-project/build4/lib -I/home/erick/llvm-project/llvm/include -I/home/erick/llvm-project/build4/include -std=c++17 -funwind-tables -frtti -D_GNU_SOURCE -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS 
CXXFLAGS = -I/home/erick/llvm-project/llvm/include -I/home/erick/llvm-project/build3/include -std=c++17  -funwind-tables -fno-rtti -D_GNU_SOURCE -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/home/erick/llvm-project/build3/lib -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMTargetParser -lLLVMSupport -lLLVMDemangle -std=c++17 -funwind-tables -frtti 
#LLVMLIBS = /home/erick/llvm-project/build3/bin/llvm-config --cxxflags --ldflags --system-libs --libs core
#clang++ -g -O3 toy.cpp `/home/erick/llvm-project/build3/bin/llvm-config --cxxflags --ldflags --system-libs --libs core` -o toy

all: $(BASE)

%.cc %.hh %.html %.gv: %.yy
	$(BISON) $(BISONFLAGS) --html --graph -o $*.cc $<

%.cc: %.ll
	$(FLEX) $(FLEXFLAGS) -o$@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o$@ $< 

$(BASE): $(BASE).o driver.o parser.o scanner.o
	$(CXX) -fuse-ld=lld  $(CXXFLAGS) $(LLVMLIBS) -lLLVMSupport -o $@ $^

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
