set -x

riscv64-unknown-elf-gcc -march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 test1.c -o test1.o
riscv64-unknown-elf-gcc -march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 test2.s -o test1.o
