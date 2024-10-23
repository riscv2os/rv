CC = riscv64-unknown-elf-gcc
CFLAGS := -march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0  -Wl,-e,main
OBJS := \
	test1.o \
	test2.o

all: $(OBJS)

test1.o: test1.c
	$(CC) $(CFLAGS) -o $@ $<

test2.o: test2.s
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(OBJS)