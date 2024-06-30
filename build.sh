cmake -S . -B build
make -C build -j
ls -al build/*.elf build/*.hex build/*.uf2