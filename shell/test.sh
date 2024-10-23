CC=riscv64-unknown-elf-gcc
CFLAGS="-march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 -Wl,-e,main"
$CC $CFLAGS ./test/test1.c -o ./test/test1.o
$CC $CFLAGS ./test/test2.s -o ./test/test2.o
rv vm32 ./test/test1.o
rv vm32 ./test/test2.o

