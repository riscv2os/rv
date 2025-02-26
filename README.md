# rv
A Set of RISC-V Tools run by command rv &lt;op> ...


## build

```
$ make
```


## test

```
$ make run_test
```

## test example

```
root@localhost:~/rv# make run_test
./build/rv0 run.u test/test1_u.c
shell:riscv64-linux-gnu-gcc -static test/test1_u.c
Hello!
shell:qemu-riscv64  ./a.out
./build/rv0 run.u test/test2_u.s
shell:riscv64-linux-gnu-gcc -static test/test2_u.s
Hello, world
shell:qemu-riscv64  ./a.out
./build/rv0 run.s test/test1_s.s
shell:riscv64-unknown-elf-gcc -march=rv32g -mabi=ilp32 -static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -T./ld/riscv32.ld test/test1_s.s
Hello.    ## hit Ctrl-A X to exit ...
QEMU: Terminated
shell:qemu-system-riscv32 -nographic -machine virt -bios none -kernel ./a.out
./build/rv0 run.0 test/test1_0.c
shell:riscv64-unknown-elf-gcc -march=rv32g -mabi=ilp32 -nostdlib -Wl,--section-start=.text=0x0 -Wl,-e,main test/test1_0.c
.rodata :起始位址 0x0 大小 0xc0
內容
0000 fd010113 I addi  x2, x2, -48
0004 02812623 S sw    x8, 44(x2)
0008 03010413 I addi  x8, x2, 48
000c fca42e23 S sw    x10, -36(x8)
0010 fe042623 S sw    x0, -20(x8)
0014 00100793 I addi  x15, x0, 1
0018 fef42423 S sw    x15, -24(x8)
001c 0200006f J jal   x0, 32
0020 fec42703 I lw    x14, -20(x8)
0024 fe842783 I lw    x15, -24(x8)
0028 00f707b3 R add   x15, x14, x15
002c fef42623 S sw    x15, -20(x8)
0030 fe842783 I lw    x15, -24(x8)
0034 00178793 I addi  x15, x15, 1
0038 fef42423 S sw    x15, -24(x8)
003c fe842703 I lw    x14, -24(x8)
0040 fdc42783 I lw    x15, -36(x8)
0044 fce7dee3 B bge   x15, x14, -36
0048 fec42783 I lw    x15, -20(x8)
004c 00078513 I addi  x10, x15, 0
0050 02c12403 I lw    x8, 44(x2)
0054 03010113 I addi  x2, x2, 48
0058 00008067 I jalr  x0, 0(x1)
005c fe010113 I addi  x2, x2, -32
0060 00112e23 S sw    x1, 28(x2)
0064 00812c23 S sw    x8, 24(x2)
0068 02010413 I addi  x8, x2, 32
006c 00a00513 I addi  x10, x0, 10
0070 f91ff0ef J jal   x1, -112
0074 fea42623 S sw    x10, -20(x8)
0078 0c000793 I addi  x15, x0, 192
007c 00100593 I addi  x11, x0, 1
0080 00078613 I addi  x12, x15, 0
0084 00000073 I ecall
0088 000017b7 U lui   x15, 1
008c 0ce7a423 S sw    x14, 200(x15)
0090 00100793 I addi  x15, x0, 1
0094 00000593 I addi  x11, x0, 0
0098 00078613 I addi  x12, x15, 0
009c 00000073 I ecall
00a0 000017b7 U lui   x15, 1
00a4 0ce7a423 S sw    x14, 200(x15)
00a8 00000793 I addi  x15, x0, 0
00ac 00078513 I addi  x10, x15, 0
00b0 01c12083 I lw    x1, 28(x2)
00b4 01812403 I lw    x8, 24(x2)
00b8 02010113 I addi  x2, x2, 32
00bc 00008067 I jalr  x0, 0(x1)


.rodata :起始位址 0xc0 大小 0x6
內容
hello/...
hello


.sbss :起始位址 0x10c8 大小 0x4

005c addi  x2, x2, -32  # x2=x2+-32=7968=0x1f20
0060 sw    x1, 28(x2)   # m[7996]=x1=0 
0064 sw    x8, 24(x2)   # m[7992]=x8=0 
0068 addi  x8, x2, 32   # x8=x2+32=8000=0x1f40
006c addi  x10, x0, 10  # x10=x0+10=10=0xa
0070 jal   x1, -112     # x[1]=116; goto 0
0000 addi  x2, x2, -48  # x2=x2+-48=7920=0x1ef0
0004 sw    x8, 44(x2)   # m[7964]=x8=8000 
0008 addi  x8, x2, 48   # x8=x2+48=7968=0x1f20
000c sw    x10, -36(x8) # m[7932]=x10=10 
0010 sw    x0, -20(x8)  # m[7948]=x0=0 
0014 addi  x15, x0, 1   # x15=x0+1=1=0x1
0018 sw    x15, -24(x8) # m[7944]=x15=1 
001c jal   x0, 32       # x[0]=32; goto 60
003c lw    x14, -24(x8) # x[14]=1
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=0
0024 lw    x15, -24(x8) # x[15]=1
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=1 
0030 lw    x15, -24(x8) # x[15]=1
0034 addi  x15, x15, 1  # x15=x15+1=2=0x2
0038 sw    x15, -24(x8) # m[7944]=x15=2 
003c lw    x14, -24(x8) # x[14]=2
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=1
0024 lw    x15, -24(x8) # x[15]=2
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=3 
0030 lw    x15, -24(x8) # x[15]=2
0034 addi  x15, x15, 1  # x15=x15+1=3=0x3
0038 sw    x15, -24(x8) # m[7944]=x15=3 
003c lw    x14, -24(x8) # x[14]=3
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=3
0024 lw    x15, -24(x8) # x[15]=3
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=6 
0030 lw    x15, -24(x8) # x[15]=3
0034 addi  x15, x15, 1  # x15=x15+1=4=0x4
0038 sw    x15, -24(x8) # m[7944]=x15=4 
003c lw    x14, -24(x8) # x[14]=4
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=6
0024 lw    x15, -24(x8) # x[15]=4
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=10 
0030 lw    x15, -24(x8) # x[15]=4
0034 addi  x15, x15, 1  # x15=x15+1=5=0x5
0038 sw    x15, -24(x8) # m[7944]=x15=5 
003c lw    x14, -24(x8) # x[14]=5
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=10
0024 lw    x15, -24(x8) # x[15]=5
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=15 
0030 lw    x15, -24(x8) # x[15]=5
0034 addi  x15, x15, 1  # x15=x15+1=6=0x6
0038 sw    x15, -24(x8) # m[7944]=x15=6 
003c lw    x14, -24(x8) # x[14]=6
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=15
0024 lw    x15, -24(x8) # x[15]=6
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=21 
0030 lw    x15, -24(x8) # x[15]=6
0034 addi  x15, x15, 1  # x15=x15+1=7=0x7
0038 sw    x15, -24(x8) # m[7944]=x15=7 
003c lw    x14, -24(x8) # x[14]=7
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=21
0024 lw    x15, -24(x8) # x[15]=7
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=28 
0030 lw    x15, -24(x8) # x[15]=7
0034 addi  x15, x15, 1  # x15=x15+1=8=0x8
0038 sw    x15, -24(x8) # m[7944]=x15=8 
003c lw    x14, -24(x8) # x[14]=8
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=28
0024 lw    x15, -24(x8) # x[15]=8
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=36 
0030 lw    x15, -24(x8) # x[15]=8
0034 addi  x15, x15, 1  # x15=x15+1=9=0x9
0038 sw    x15, -24(x8) # m[7944]=x15=9 
003c lw    x14, -24(x8) # x[14]=9
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=36
0024 lw    x15, -24(x8) # x[15]=9
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=45 
0030 lw    x15, -24(x8) # x[15]=9
0034 addi  x15, x15, 1  # x15=x15+1=10=0xa
0038 sw    x15, -24(x8) # m[7944]=x15=10 
003c lw    x14, -24(x8) # x[14]=10
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # goto 32
0020 lw    x14, -20(x8) # x[14]=45
0024 lw    x15, -24(x8) # x[15]=10
0028 add   x15, x14, x15        # 
002c sw    x15, -20(x8) # m[7948]=x15=55 
0030 lw    x15, -24(x8) # x[15]=10
0034 addi  x15, x15, 1  # x15=x15+1=11=0xb
0038 sw    x15, -24(x8) # m[7944]=x15=11 
003c lw    x14, -24(x8) # x[14]=11
0040 lw    x15, -36(x8) # x[15]=10
0044 bge   x15, x14, -36        # 
0048 lw    x15, -20(x8) # x[15]=55
004c addi  x10, x15, 0  # x10=x15+0=55=0x37
0050 lw    x8, 44(x2)   # x[8]=8000
0054 addi  x2, x2, 48   # x2=x2+48=7968=0x1f20
0058 jalr  x0, 0(x1)    # x[0]=pc+4, goto 0x74
0074 sw    x10, -20(x8) # m[7980]=x10=55 
0078 addi  x15, x0, 192 # x15=x0+192=192=0xc0
007c addi  x11, x0, 1   # x11=x0+1=1=0x1
0080 addi  x12, x15, 0  # x12=x15+0=192=0xc0
0084 ecall      # 

0088 lui   x15, 1       # x[15]=1
008c sw    x14, 200(x15)        # m[4296]=x14=11 
0090 addi  x15, x0, 1   # x15=x0+1=1=0x1
0094 addi  x11, x0, 0   # x11=x0+0=0=0x0
0098 addi  x12, x15, 0  # x12=x15+0=1=0x1
009c ecall      # exit(1)
shell:./build/vm32 ./a.out
```