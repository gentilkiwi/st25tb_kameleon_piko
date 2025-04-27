cmake -S . -B build
cmake --build build --parallel $(nproc)
ls -al build/*.elf build/*.hex build/*.uf2
