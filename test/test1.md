直接把 .text .rodata .sbss 載入到 LMA 去，這樣位址應該就是對的

(base) cccimac@cccimacdeiMac rv % rv dump -h a.out

a.out:     file format elf32-littleriscv

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         000000a8  00000000  00000000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .rodata       00000006  000000a8  000000a8  000010a8  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .sbss         00000004  000010b0  000010b0  000010b0  2**2
                  ALLOC
  3 .comment      00000019  00000000  00000000  000010ae  2**0
                  CONTENTS, READONLY
  4 .riscv.attributes 0000004e  00000000  00000000  000010c7  2**0
                  CONTENTS, READONLY
shell:riscv64-unknown-elf-objdump -h a.out 


(base) cccimac@cccimacdeiMac rv % rv dump -s a.out   

a.out:     file format elf32-littleriscv

Contents of section .text:
 0000 130101fd 23268102 13040103 232ea4fc  ....#&......#...
 0010 232604fe 93071000 2324f4fe 6f000002  #&......#$..o...
 0020 0327c4fe 832784fe b307f700 2326f4fe  .'...'......#&..
 0030 832784fe 93871700 2324f4fe 032784fe  .'......#$...'..
 0040 8327c4fd e3dee7fc 8327c4fe 13850700  .'.......'......
 0050 0324c102 13010103 67800000 130101fe  .$......g.......
 0060 232e1100 232c8100 13040102 1305a000  #...#,..........
 0070 eff01ff9 2326a4fe 9307800a 93051000  ....#&..........
 0080 93070600 73000000 b7170000 23a8e70a  ....s.......#...
 0090 93070000 13850700 8320c101 03248101  ......... ...$..
 00a0 13010102 67800000                    ....g...        
Contents of section .rodata:
 00a8 68656c6c 6f00                        hello.          
Contents of section .comment:
 0000 4743433a 20286763 38393164 38646332  GCC: (gc891d8dc2
 0010 29203133 2e322e30 00                 ) 13.2.0.       
Contents of section .riscv.attributes:
 0000 414d0000 00726973 63760001 43000000  AM...riscv..C...
 0010 04100572 76333269 3270315f 6d327030  ...rv32i2p1_m2p0
 0020 5f613270 315f6632 70325f64 3270325f  _a2p1_f2p2_d2p2_
 0030 7a696373 72327030 5f7a6966 656e6365  zicsr2p0_zifence
 0040 69327030 5f7a6d6d 756c3170 3000      i2p0_zmmul1p0.  
shell:riscv64-unknown-elf-objdump -s a.out 


(base) cccimac@cccimacdeiMac rv % rv dump -d a.out         

a.out:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <sum>:
   0:   fd010113                add     sp,sp,-48
   4:   02812623                sw      s0,44(sp)
   8:   03010413                add     s0,sp,48
   c:   fca42e23                sw      a0,-36(s0)
  10:   fe042623                sw      zero,-20(s0)
  14:   00100793                li      a5,1
  18:   fef42423                sw      a5,-24(s0)
  1c:   0200006f                j       3c <sum+0x3c>
  20:   fec42703                lw      a4,-20(s0)
  24:   fe842783                lw      a5,-24(s0)
  28:   00f707b3                add     a5,a4,a5
  2c:   fef42623                sw      a5,-20(s0)
  30:   fe842783                lw      a5,-24(s0)
  34:   00178793                add     a5,a5,1
  38:   fef42423                sw      a5,-24(s0)
  3c:   fe842703                lw      a4,-24(s0)
  40:   fdc42783                lw      a5,-36(s0)
  44:   fce7dee3                bge     a5,a4,20 <sum+0x20>
  48:   fec42783                lw      a5,-20(s0)
  4c:   00078513                mv      a0,a5
  50:   02c12403                lw      s0,44(sp)
  54:   03010113                add     sp,sp,48
  58:   00008067                ret

0000005c <main>:
  5c:   fe010113                add     sp,sp,-32
  60:   00112e23                sw      ra,28(sp)
  64:   00812c23                sw      s0,24(sp)
  68:   02010413                add     s0,sp,32
  6c:   00a00513                li      a0,10
  70:   f91ff0ef                jal     0 <sum>
  74:   fea42623                sw      a0,-20(s0)
  78:   0a800793                li      a5,168
  7c:   00700893                li      a7,7
  80:   00080793                mv      a5,a6
  84:   00000073                ecall
  88:   000017b7                lui     a5,0x1
  8c:   0ae7a823                sw      a4,176(a5) # 10b0 <temp>
  90:   00000793                li      a5,0
  94:   00078513                mv      a0,a5
  98:   01c12083                lw      ra,28(sp)
  9c:   01812403                lw      s0,24(sp)
  a0:   02010113                add     sp,sp,32
  a4:   00008067                ret
shell:riscv64-unknown-elf-objdump -d a.out 