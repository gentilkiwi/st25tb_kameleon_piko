cmake -S . -B build
cmake --build build --parallel
ls -al build/*.elf build/*.hex build/*.uf2