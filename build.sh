rm -rf build
mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=True      -DLLVM_DIR=/home/dtomassi/test-llvm/build/lib/cmake/llvm/      ../
make
