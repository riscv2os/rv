#include <stdio.h>
#include "lib.h"
#include "dasm.h"
#include "riscv.h"

#define MEM_SIZE 10000
int32_t pc, pc_new;
int32_t reg[32];
int32_t memory[MEM_SIZE];

#define SYS_PUTS 1

void ecall() { // 系統呼叫
    if (reg[A1]==SYS_PUTS) {
        // printf("%s", (char*)(uint64_t)reg[A1]);
        printf("\necall(%d):print string in a2=x%d=%d\n", reg[A1], A2, reg[A2]);
    } else {
        printf("\nunhandled ecall a1=x%d=%d\n", reg[A1], A1);
    }
}

#define ebreak() // ebreak 為 debugger 用途，本程式沒處理

#define lb(rd, rs, imm)  reg[rd]=*(int8_t*)&memory[reg[rs]+imm]; log("x[%d]=%d", rd, *(int8_t*)&memory[reg[rs]+imm])
#define lh(rd, rs, imm)  reg[rd]=*(int16_t*)&memory[reg[rs]+imm]; log("x[%d]=%d", rd, *(int16_t*)&memory[reg[rs]+imm])
#define lw(rd, rs, imm)  reg[rd]=*(int32_t*)&memory[reg[rs]+imm]; log("x[%d]=%d", rd, *(int32_t*)&memory[reg[rs]+imm])  
#define lbu(rd, rs, imm) reg[rd]=*(uint8_t*)&memory[reg[rs]+imm]; log("x[%d]=%d", rd, *(uint8_t*)&memory[reg[rs]+imm])
#define lhu(rd, rs, imm) reg[rd]=*(uint16_t*)&memory[reg[rs]+imm]; log("x[%d]=%d", rd, *(uint16_t*)&memory[reg[rs]+imm])

#define addi(rd, rs, imm) reg[rd]=reg[rs]+imm; log("x%d=x%d+%d=%d", rd, rs, imm, reg[rd])
#define slti(rd, rs, imm) reg[rd]=reg[rs]<<imm
#define sltiu(rd, rs, imm) reg[rd]=reg[rs]<<imm // bug
#define xori(rd, rs, imm) reg[rd]=reg[rs]^imm
#define ori(rd, rs, imm)  reg[rd]=reg[rs]|imm
#define andi(rd, rs, imm) reg[rd]=reg[rs]&imm

#define beq(rs1, rs2, imm)  if (reg[rs1] == reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }
#define bne(rs1, rs2, imm)  if (reg[rs1] != reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }
#define blt(rs1, rs2, imm)  if (reg[rs1] <  reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }
#define bge(rs1, rs2, imm)  if (reg[rs1] >= reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }
#define bltu(rs1, rs2, imm) if ((uint32_t)reg[rs1] < (uint32_t)reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }
#define bgeu(rs1, rs2, imm) if ((uint32_t)reg[rs1] >= (uint32_t)reg[rs2]) { pc_new = pc+imm; log("goto %d", pc_new); }

#define add(rd, rs1, rs2) reg[rd]=reg[rs1]+reg[rs2]
#define sub(rd, rs1, rs2) reg[rd]=reg[rs1]-reg[rs2]
#define sll(rd, rs1, rs2) reg[rs1] << (reg[rs2]&0x1F); 
#define slt(rd, rs1, rs2) reg[rd]=reg[rs1]<reg[rs2]
#define sltu(rd, rs1, rs2) (uint32_t)reg[rs1] < (uint32_t)reg[rs2]
#define xor(rd, rs1, rs2) reg[rd]=reg[rs1]^reg[rs2]
#define srl(rd, rs1, rs2) (uint32_t)reg[rs1] >> (reg[rs2]&0x1F)
#define sra(rd, rs1, rs2) reg[rs1] >> (reg[rs2]&0x1F)
#define or(rd, rs1, rs2) reg[rd]=reg[rs1]|reg[rs2]
#define and(rd, rs1, rs2) reg[rd]=reg[rs1]&reg[rs2]

#define sb(rs1, rs2, imm) *(uint8_t*)&memory[reg[rs1]+imm]  = (uint8_t)reg[rs2];  log("m[%d]=x%d=%d ", reg[rs1]+imm, rs2, (int8_t)reg[rs2])
#define sh(rs1, rs2, imm) *(uint16_t*)&memory[reg[rs1]+imm] = (uint16_t)reg[rs2]; log("m[%d]=x%d=%d ", reg[rs1]+imm, rs2, (int16_t)reg[rs2])
#define sw(rs1, rs2, imm) *(uint32_t*)&memory[reg[rs1]+imm] = (uint32_t)reg[rs2]; log("m[%d]=x%d=%d ", reg[rs1]+imm, rs2, (int32_t)reg[rs2])

#define lui(rd, imm)   reg[rd] = (imm & 0xFFFFF) << 12;log("x[%d]=%d", rd, (imm & 0xFFFFF))

#define auipc(rd, imm) reg[rd] = (imm & 0xFFFFF) << 12 | (pc & 0xFFF); log("x[%d]=0x%x", rd, reg[rd])
#define jalr(rd, rs, imm) reg[rd]=pc+4; pc_new=(reg[rs]+imm) & ~0x3; log("x[%d]=pc+4, goto 0x%x", rd, pc_new)
// & ~0x3 是為了保證 4byte 對齊
// call = auipc ra, offset_upper; jalr ra, offset_lower(ra)

#define jal(rd, imm) reg[rd] = pc+4; pc_new = pc+imm; log("x[%d]=%d; goto %d", rd, pc+4, pc_new);

#include "rv32do.c"

void exec_instr(uint32_t instr) {
    char asm1[100];
    disassemble(instr, asm1);
    printf("%04x %s\t# ", pc, asm1);
    reg[0] = 0; // x0 is always 0;
    do_instr(instr);
    printf("\n");
}

#define STACK_START 8000

void vm_run(char *memory, int size, int entry)
{
    pc = entry;
    reg[SP] = STACK_START;
    while (pc < size-4)
    {
        uint32_t instr = decode_little_endian32(&memory[pc]);
        pc_new = -1;
        exec_instr(instr);
        pc = (pc_new == -1)?pc+4:pc_new;
    }
}
