#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC "riscv64-unknown-elf-gcc"
#define CFLAG32 "-march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 -Wl,-e,main"
#define VM "./build/vm32"
char shell_cmd[1000];

#define shell(...) sprintf(shell_cmd, __VA_ARGS__); system(shell_cmd); printf("shell:"); printf(__VA_ARGS__); printf("\n")

void dispatch(char *op, char *args[], int argc) {
    char arg_tail[1000], *p=arg_tail;

    for (int i=0; i<argc; i++) {
        sprintf(p, "%s ", args[i]);
        p += strlen(p);
    }

    if (strcmp(op, "cc32")==0) {
        shell("%s %s %s", CC, CFLAG32, arg_tail);
    }
    if (strcmp(op, "run32")==0) {
        shell("%s %s %s", CC, CFLAG32, args[0]);
        shell("%s %s", VM, "./a.out");        
    }
}

int main(int argc, char *argv[]) {
    char *op = argv[1];

    dispatch(op, &argv[2], argc-2);
}
