

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