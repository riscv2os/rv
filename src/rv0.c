#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_S "riscv64-unknown-elf-gcc"
#define CC_U "riscv64-linux-gnu-gcc"
#define CC CC_U
#define CC_0 CC_S

// "gcc-riscv64-linux-gnu"
#define CFLAG_U "-static"
#define CFLAG_S ""
#define CFLAG CFLAG_U
#define CFLAG_0 "-march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 -Wl,-e,main"

#define VM_S "qemu-system-riscv32" 
#define VM_U "qemu-riscv64"
// #define VM_U "qemu-riscv32-static" 
#define VM VM_U 
#define VM_0 "./build/vm32"

#define DUMP_S "riscv64-unknown-elf-objdump"
#define DUMP_U "riscv64-linux-gnu-objdump"
#define DUMP DUMP_U
#define DUMP_0 "./build/dump0"

#define SETUP "./shell/setup.sh"
#define TEST "./shell/test.sh"

char shell_cmd[1024];

#define shell(...)                   \
    sprintf(shell_cmd, __VA_ARGS__); \
    system(shell_cmd);               \
    printf("shell:");                \
    printf(__VA_ARGS__);             \
    printf("\n")

void dispatch(char *op, char *args[], int argc)
{
    char arg_tail[800], *p = arg_tail;

    for (int i = 0; i < argc; i++)
    {
        sprintf(p, "%s ", args[i]);
        p += strlen(p);
    }

    if (strcmp(op, "cc") == 0)
    {
        shell("%s %s %s", CC, CFLAG, arg_tail);
    }
    else if (strcmp(op, "cc.u") == 0)
    {
        shell("%s %s %s", CC_U, CFLAG_U, arg_tail);
    }
    else if (strcmp(op, "cc.s") == 0)
    {
        shell("%s %s %s", CC_S, CFLAG_S, arg_tail);
    }
    else if (strcmp(op, "cc.0") == 0)
    {
        shell("%s %s %s", CC_0, CFLAG_0, arg_tail);
    }
    else if (strcmp(op, "dump") == 0)
    {
        shell("%s %s", DUMP, arg_tail);
    }
    else if (strcmp(op, "dump.0") == 0)
    {
        shell("%s %s", DUMP_0, arg_tail);
    }
    else if (strcmp(op, "run.0") == 0)
    {
        shell("%s %s %s", CC_0, CFLAG_0, args[0]);
        shell("%s %s", VM_0, "./a.out");
    }
    else if (strcmp(op, "run.u") == 0)
    {
        shell("%s %s %s", CC_U, CFLAG_U, args[0]);
        shell("%s %s", VM_U, "./a.out");
    }
    else if (strcmp(op, "vm.0") == 0)
    {
        shell("%s %s", VM_0, arg_tail);
    }
    else if (strcmp(op, "vm.u") == 0)
    {
        shell("%s %s", VM_U, arg_tail);
    }
    else if (strcmp(op, "vm.s") == 0)
    {
        shell("%s %s", VM_S, arg_tail);
    }
    else if (strcmp(op, "setup") == 0)
    {
        shell("%s", SETUP);
    }
    else if (strcmp(op, "test") == 0)
    {
        shell("%s", TEST);
    }
    else
    {
        perror("rv0 <op> not found error!\n");
    }
}

int main(int argc, char *argv[])
{
    char *op = argv[1];
    if (argc < 2)
    {
        perror("rv <op> ... // 必須有 <op>\n");
        return 1;
    }
    dispatch(op, &argv[2], argc - 2);
}
