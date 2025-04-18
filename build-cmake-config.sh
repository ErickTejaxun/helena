Opciones: 
	-G "Ninja" u otra herramienta de construccion. 
	-DDLV_USE_LINKER=gold/lld que son opciones más rápidas que BFD linker (default).
		-LLVM_PARALLEL_LINK_JOBS=N para ajustar el máximo número de threads. 


cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
-DLLVM_EXTERNAL_PROJECTS=helena \
-DLLVM_EXTERNAL_HELENA_SOURCE_DIR=./../helena -DCMAKE_INSTALL_PREFIX=../llvm-usac ../llvm

cmake --build . 


cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_EXTERNAL_PROJECTS=helena -DLLVM_EXTERNAL_HELENA_SOURCE_DIR=./../helena -DCMAKE_INSTALL_PREFIX=../llvm-12 ../llvm

../llvm-12/bin/helena 


cmake -G Ninja ../llvm \
        -DLLVM_PARALLEL_COMPILE_JOBS=7 \
        -DLLVM_PARALLEL_LINK_JOBS=1 \
        -DLLVM_BUILD_EXAMPLES=ON \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DCMAKE_BUILD_TYPE=Release  
        -DCMAKE_BUILD_TYPE=Debug faster 
        -DCMAKE_BUILD_TYPE=RelWithDebInfo faster than release
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_CCACHE_BUILD=ON \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DLLVM_ENABLE_PROJECTS='claneg;lldb;lld;mlir;clang-tools-extra;compiler-rt;libcxx;libcxxabi;libunwind;polly' \
        -DCMAKE_C_COMPILER=clang \       # If you have clang installed already
        -DCMAKE_CXX_COMPILER=clang++ \   # If you have clang installed already
        -DLLVM_USE_LINKER=gold;lld 
        -DBUILD_SHARED_LIBS=ON #Activating Dynamic library
        -DLLVM_USE_SPLIT_DWARF=ON #Activa el sistema de fragmentación. Util para recompilar CLANG and GCC
        -DLLVM_OPTIMIZED_TABLEGEN=ON # Crea una versión optimizada para llvm-tblgen (útil cuando no estamos desarrollando en table gen)
        -DLLVM_USE_NEWPM=ON # env CC=`which clang` CXX=`which clang++` Hay que establecer primero las variables de entorno



sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 180
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 180

CC=clang-18
cmake -G Ninja ../llvm \
        -DLLVM_PARALLEL_COMPILE_JOBS=7 \
        -DLLVM_PARALLEL_LINK_JOBS=1 \
        -DLLVM_BUILD_EXAMPLES=ON \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_CCACHE_BUILD=OFF \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
        -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;polly;openmp;pstl;flang;libc;libclc' \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DLLVM_USE_LINKER=gold;lld \
        -DBUILD_SHARED_LIBS=ON \
        -DLLVM_USE_SPLIT_DWARF=ON \
        -DLLVM_OPTIMIZED_TABLEGEN=ON \
        -DLLVM_USE_NEWPM=ON \
        -DLLVM_ENABLE_ABI_BREAKING_CHECKS=ON \
        -DLIBOMP_OMPD_SUPPORT=OFF


cmake -G Ninja ../llvm \
        -DLLVM_PARALLEL_COMPILE_JOBS=7 \
        -DLLVM_PARALLEL_LINK_JOBS=1 \
        -DLLVM_BUILD_EXAMPLES=ON \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_CCACHE_BUILD=OFF \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
        -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;polly' \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DLLVM_USE_LINKER=gold;lld \
        -DBUILD_SHARED_LIBS=ON \
        -DLLVM_USE_SPLIT_DWARF=ON \
        -DLLVM_OPTIMIZED_TABLEGEN=ON \
        -DLLVM_USE_NEWPM=ON \
        -DLLVM_ENABLE_ABI_BREAKING_CHECKS=ON 


-DLIBOMP_OMPD_SUPPORT=OFF error con python 3.12

cmake -G Ninja ../llvm  -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;polly' -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++



cmake -G "Ninja" ../llvm -DCMAKE_INSTALL_PREFIX="$LLVMINS" -DCMAKE_BUILD_TYPE="Release" -DLLVM_TARGETS_TO_BUILD="X86;ARM;Mips;AArch64;WebAssembly;PowerPC" -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;polly' -DLLVM_ENABLE_CXX1Y=ON  -DLLVM_ENABLE_ASSERTIONS=ON -DSPHINX_OUTPUT_HTML=OFF -DSPHINX_OUTPUT_MAN=OFF "$LLVMSRC"

cmake -G Ninja ../llvm -DLLVM_BUILD_EXAMPLES=ON  -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;libcxx;libcxxabi;libunwind;polly' -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

cmake -G Ninja ../llvm -DLLVM_BUILD_EXAMPLES=ON -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;libcxx;libcxxabi;libunwind;polly' -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11


/usr/bin/gcc
/usr/bin/g++

cmake -G Ninja ../llvm -DLLVM_BUILD_EXAMPLES=ON -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;libcxx;libcxxabi;libunwind;polly' -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++



--- Used to build the build_debugin
cmake -G Ninja ../llvm  -DLLVM_TARGETS_TO_BUILD="X86" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_PROJECTS='clang;lldb;lld;mlir;clang-tools-extra;compiler-rt;polly' -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi"
ninja -j8 all 

  cmake --build .

  cmake --build . --target check-mlir

  cmake -DCMAKE_INSTALL_PREFIX=/your/target/location -P cmake_install.cmake



sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 180

/usr/local/bin/ld.lld

sudo update-alternatives --install /usr/local/bin/ld.lld ld.lld /home/erick/llvm-project/build3/bin/ld.lld 180
sudo update-alternatives --install /usr/local/bin/ld.bfd ld.bfd /home/erick/llvm-project/build3/bin/ld.bfd 180
sudo update-alternatives --install /usr/local/bin/ld.gold ld.gold /home/erick/llvm-project/build3/bin/ld.gold 180
sudo update-alternatives --install /usr/local/bin/ld.so ld.so /home/erick/llvm-project/build3/bin/ld.so 180

ld.bfd     ld.gold    ld.lld     ld.lld-14  ld.lld-18  ld.so