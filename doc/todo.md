## 未解決問題

像 puts("hello") 這樣的字串，定址會有問題，如何解決？

=> 將 data 定在某個位址開始，像 stack 一樣，然後用特定暫存器指向該位址。


直接把 .text .rodata .sbss 載入到 LMA 去，這樣位址應該就是對的

(base) cccimac@cccimacdeiMac rv % rv run32 test/test1puts.c 
...

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

## 已解決問題

main, sum 這樣的函數指向不同位址?? NO


vm_test 時

call sum // sum 位址為 0

結果得到

```
0070 auipc x1, 0        # x[1]=0x70
0074 jalr  x1, 0(x1)    # x[1]=pc+4, goto 0x78
0078 sw    x10, -20(x8) # m[7980]=x10=10 
```

問題是按照下列實作，這樣不會跳轉到 0 ，而是根本不會跳。（問題在哪？）

```c
void auipc(VM* vm, uint8_t rd, uint32_t imm) {
    // 獲取當前 PC 的值
    uint32_t current_pc = vm->pc;

    // 計算高 20 位立即數的值
    uint32_t upper_imm = (imm << 12) & 0xFFFFF000; // 擴展至 32 位

    // 將結果存儲到指定寄存器
    vm->registers[rd] = current_pc + upper_imm;
}
```

解決了，因為 gcc 設定 -c 只編譯不連結，所以 objdump 反組譯就錯了。

拿掉 -c 改只用 -nostdlib ，然後加上  -Wl,--section-start=.text=0x0 從 0 開始，就可以用了。


