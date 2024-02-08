cmake \
    -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_EXTERNAL_PROJECTS=helena \
    -DLLVM_EXTERNAL_HELENA_SOURCE_DIR=./../../helena \
    -DCMAKE_INSTALL_PREFIX=../../llvm-12 ../../llvm