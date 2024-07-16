set(PROJECT st25tb_kameleon_piko)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico)
set(PICO_COMPILER pico_arm_gcc)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)
project(${PROJECT} C CXX ASM)

pico_sdk_init()

add_executable(${PROJECT}
    st25tb_kameleon_piko.c
    
    kameleon/board.c
    kameleon/leds.c
    kameleon/slots.c
    kameleon/trf7970a.c
    
    kameleon/st25tb/st25tb.c
    kameleon/st25tb/st25tb_target.c
    kameleon/st25tb/st25tb_initiator.c
    
    kameleon/modes/emulate.c
    kameleon/modes/rewrite.c
    kameleon/modes/detect.c
    kameleon/modes/select.c
    kameleon/modes/learn.c
    kameleon/modes/unk.c
)

pico_set_program_name(${PROJECT} ${PROJECT})
pico_set_program_description(${PROJECT} "st25tb_kameleon on Raspberry Pi Pico")
pico_set_program_version(${PROJECT} "0.1")
pico_set_program_url(${PROJECT} "https://github.com/gentilkiwi/st25tb_kameleon_piko")

pico_enable_stdio_uart(${PROJECT} 0)
pico_enable_stdio_usb(${PROJECT} 1)

pico_set_linker_script(${PROJECT} ${CMAKE_SOURCE_DIR}/memmap_2M_reserved.ld)

target_compile_options(${PROJECT} PRIVATE -Werror -Wall -Wextra)
target_link_libraries(${PROJECT}
    pico_stdlib
    pico_unique_id
    hardware_spi
    hardware_pio
    hardware_flash
)

pico_add_hex_output(${PROJECT})
pico_add_uf2_output(${PROJECT})
