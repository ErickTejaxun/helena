# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

# ==========================================
# OS DETECTION AND PATH CONFIGURATION
# ==========================================
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # Rutas para Linux (Tus rutas originales)
    LLVM_SRC_INCLUDE_PATH = /home/erick/llvm-project/llvm/include
    LLVM_BUILD_INCLUDE_PATH = /home/erick/llvm-project/build3/include
    LLVM_BUILD_LIB_PATH = /home/erick/llvm-project/build3/lib
    
    # Rutas extra que tenías definidas
    MLIR_INCLUDE_PATH = /home/erick/llvm-project/llvm/include/
    LLVM_INCLUDE_PATH = /home/erick/llvm-project/llvm/include/
    LLVM_C_INCLUDE_PATH = /home/erick/llvm-project/build4/include/
    LLVM_CURRENT_PATH = /home/erick/llvm-project/llvm/include/
endif

ifeq ($(UNAME_S),Darwin)
    # Rutas para macOS
    LLVM_SRC_INCLUDE_PATH = /Users/erick/llvm-project/llvm/include
    LLVM_BUILD_INCLUDE_PATH = /Users/erick/llvm-project/build/include
    LLVM_BUILD_LIB_PATH = /Users/erick/llvm-project/build/lib
    
    # Rutas extra que tenías definidas
    MLIR_INCLUDE_PATH = /Users/erick/llvm-project/mlir/include/
    LLVM_INCLUDE_PATH = /Users/erick/llvm-project/llvm/include/
    LLVM_C_INCLUDE_PATH = /Users/erick/llvm-project/build/include/
    LLVM_CURRENT_PATH = /Users/erick/llvm-project/llvm/include/
endif

# ==========================================
# TOOLCHAIN CONFIGURATION
# ==========================================
BASE = helen-frontend++
BISON = bison
CXX = clang++
FLEX = flex

# ==========================================
# FLAGS
# ==========================================
# Se inyectan las variables LLVM_SRC_INCLUDE_PATH, LLVM_BUILD_INCLUDE_PATH y LLVM_BUILD_LIB_PATH
CXXFLAGS = -I$(LLVM_SRC_INCLUDE_PATH) -I$(LLVM_BUILD_INCLUDE_PATH) -std=c++17 -funwind-tables -fno-rtti -D_GNU_SOURCE -D_DEBUG -D_GLIBCXX_ASSERTIONS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L$(LLVM_BUILD_LIB_PATH) -lLLVMCore -lLLVMRemarks -lLLVMBitstreamReader -lLLVMBinaryFormat -lLLVMTargetParser -lLLVMSupport -lLLVMDemangle -std=c++17 -funwind-tables -frtti 

# ==========================================
# TARGETS
# ==========================================
all: $(BASE)

%.cc %.hh %.html %.gv: %.yy
	$(BISON) $(BISONFLAGS) --html --graph -o $*.cc $<

%.cc: %.lex
	$(FLEX) $(FLEXFLAGS) -o$@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o$@ $< 

#Agregamos las banderas -Wl, -rpath para que el compilador guarde las direcciones de las librerias. 
#Si no, no las encuentra. 
$(BASE): $(BASE).o driver.o parser.o scanner.o
	$(CXX) -fuse-ld=lld $(CXXFLAGS) $(LLVMLIBS) -lLLVMSupport -Wl,-rpath,$(LLVM_BUILD_LIB_PATH) -o $@ $^

$(BASE).o: parser.hh
parser.o: parser.hh
scanner.o: parser.hh

run: $(BASE)
	@echo "Type arithmetic expressions.  Quit with ctrl-d."
	./$< -

CLEANFILES =                                        \
  *.o                                       \
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh    \
  scanner.cc *.ll

clean:
	rm -f $(CLEANFILES)