#include <stdio.h>
#include "lib.h"
#include "dasm.h"
#include "riscv.h"

#define log printf

#define MEM_SIZE 10000
int32_t pc, pc_new;
int32_t reg[32];
int32_t memory[MEM_SIZE];

#define SYS_PUTS 1

void ecall() { // 系統呼叫
    if (reg[A0]==SYS_PUTS) {
        // printf("%s", (char*)(uint64_t)reg[A1]);
        printf("\nprint string in a1=%d", A1);
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

/*
// 取得 I 型指令
void exec_i_type(uint32_t instr)
{
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;     // 目的暫存器
    uint32_t funct3 = (instr >> 12) & 0x7; // 功能碼
    uint32_t rs1 = (instr >> 15) & 0x1F;   // 原暫存器1
    int32_t imm = (instr >> 20);           // 立即數
    imm = sign_extend_12(imm); // 符號擴展

    if (opcode == 0x67)
    {
        printf("jalr x%d, x%d, %d", rd, rs1, imm);
        return;
    }
    else if (opcode == 0x73)
    {
        if (imm == 0)
            ecall();
        else if (imm == 1)
            ebreak();
        return;
    }
    else if (opcode == 0x03)
    {
        switch (funct3)
        {
        case 0x0: // lb
            printf("lb x%d, %d(x%d)", rd, imm, rs1);
            break;
        case 0x1: // lh
            printf("lh x%d, %d(x%d)", rd, imm, rs1);
            break;
        case 0x2: // lw
            printf("lw x%d, %d(x%d)", rd, imm, rs1);
            break;
        case 0x4: // lbu
            printf("lbu x%d, %d(x%d)", rd, imm, rs1);
            break;
        case 0x5: // lhu
            printf("lhu x%d, %d(x%d)", rd, imm, rs1);
            break;
        default:
            printf("Unknown I-Type");
            break;
        }
    }
    else if (opcode == 0x13)
    {
        switch (funct3)
        {
        case 0x0:
            reg[rd] = reg[rs1]+imm; // addi rd, imm(rs1)
            printf("x%d=x%d+%d=%d", rd, rs1, imm, reg[rd]);
            break;
        case 0x2: // slti
            reg[rd] = reg[rs1] < imm;
            break;
        case 0x3: // sltiu
            reg[rd] = (uint32_t) reg[rs1] < (uint32_t) imm;
            break;
        case 0x4: // xori
            reg[rd] = reg[rs1] ^ imm;
            break;
        case 0x6: // ori
            reg[rd] = reg[rs1] | imm;
            break;
        case 0x7: // andi
            reg[rd] = reg[rs1] | imm;
            break;
        default:
            perror("Unknown I-Type");
            break;
        }
    }
}

// 取得 Branch 型指令
void exec_b_type(uint32_t instr)
{
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    int32_t imm = ((instr >> 31) & 1) << 12 |
                  ((instr >> 7) & 1) << 11 |
                  ((instr >> 25) & 0x3F) << 5 |
                  ((instr >> 8) & 0xF) << 1; 
    imm = sign_extend_12(imm); // 符號擴展

    switch (funct3)
    {
    case 0x0: // BEQ
        if (reg[rs1] == reg[rs2]) pc_new = pc+imm;
        break;
    case 0x1: // BNE
        if (reg[rs1] != reg[rs2]) pc_new = pc+imm;
        break;
    case 0x4: // BLT
        if (reg[rs1] < reg[rs2]) pc_new = pc+imm;
        break;
    case 0x5: // BGE
        if (reg[rs1] >= reg[rs2]) pc_new = pc+imm;
        break;
    case 0x6: // BLTU
        if ((uint32_t)reg[rs1] < (uint32_t)reg[rs2]) pc_new = pc+imm;
        break;
    case 0x7: // BGEU
        if ((uint32_t)reg[rs1] == (uint32_t)reg[rs2]) pc_new = pc+imm;
        break;
    default:
        perror("Unknown B-Type");
        break;
    }
}

// 取得 R 型指令
void exec_r_type(uint32_t instr)
{
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;

    if (funct7 == 0x00 && funct3 == 0x00)
    {
        reg[rd] = reg[rs1]+reg[rs2]; // add
    }
    else if (funct7 == 0x20 && funct3 == 0x00)
    {
        reg[rd] = reg[rs1]-reg[rs2]; // sub
    }
    else if (funct3 == 0x01)
    {
        reg[rd] = reg[rs1] << (reg[rs2]&0x1F); // sll (Shift Left Logical)
    }
    else if (funct3 == 0x02)
    {
        reg[rd] = reg[rs1] < reg[rs2]; // slt (Set if Less Than)
    }
    else if (funct3 == 0x03)
    {
        reg[rd] = (uint32_t)reg[rs1] < (uint32_t)reg[rs2]; // sltu (Set if Less Than Unsigned)
    }
    else if (funct3 == 0x04)
    {
        reg[rd] = reg[rs1]^reg[rs2]; // xor
    }
    else if (funct3 == 0x05 && funct7 == 0x00)
    {
        reg[rd] = (uint32_t)reg[rs1] >> (reg[rs2]&0x1F); // srl (Shift Right Logical)
    }
    else if (funct3 == 0x05 && funct7 == 0x20)
    {
        reg[rd] = reg[rs1] >> (reg[rs2]&0x1F); // sra (Shift Right Arithmatic)
    }
    else if (funct3 == 0x06)
    {
        reg[rd] = reg[rs1] | reg[rs2]; // or
    }
    else if (funct3 == 0x07)
    {
        reg[rd] = reg[rs1] & reg[rs2]; // and
    }
    else
    {
        perror("Unknown R-Type");
    }
}

// 取得 S 型指令
void exec_s_type(uint32_t instr)
{
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
    imm = sign_extend_12(imm);

    if (funct3 == 0x0)
    {
        *(uint8_t*)&memory[reg[rs1]+imm] = (uint8_t)reg[rs2]; // sb
    }
    else if (funct3 == 0x1)
    {
        *(uint16_t*)&memory[reg[rs1]+imm] = (uint16_t)reg[rs2]; // sh
    }
    else if (funct3 == 0x2)
    {
        memory[reg[rs1]+imm] = reg[rs2]; // sw rs2, imm(rs1)
        printf("m[%d]=x%d=%d ", reg[rs1]+imm, rs2, reg[rs2]);
    }
    else
    {
        perror("Unknown S-Type");
    }
}

// 取得 U 型指令
void exec_u_type(uint32_t instr)
{
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    int32_t imm = (instr & 0xFFFFF000) >> 12; // 高20位立即數

    if (opcode == 0x37)
    {
        reg[rd] = (imm & 0xFFFFF) << 12;  // lui
    }
    else if (opcode == 0x17)
    { 
        reg[rd] = (imm & 0xFFFFF) << 12 | (pc & 0xFFF); // auipc
        // 取低 20 位並與低 12 位的 PC 相加
    }
    else
    {
        perror("Unknown U-Type");
    }
}

// 取得j 型指令
void exec_j_type(uint32_t instr)
{
    uint32_t rd = (instr >> 7) & 0x1F;
    int32_t imm = ((instr >> 12) & 0xFF) << 12 |
                  ((instr >> 20) & 1) << 11 |
                  ((instr >> 21) & 0x3FF) << 1;
    imm = sign_extend_20(imm);

    // JAL
    reg[rd] = pc + 4; // 當前指令的下一地址
    pc_new = pc + imm; // 計算跳轉地址
}

// 根據指令類型調用對應的反組譯函數
void exec(uint32_t instr)
{
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;

    reg[0] = 0;
    char asm1[100];
    char type = disassemble(instr, asm1);
    printf("%04x %s\t# ", pc, asm1);

    switch (opcode)
    {
    case 0x03: // Load
    case 0x13: // I-Type instructions
    case 0x67: // JALR
    case 0x73: // ECALL
        exec_i_type(instr);
        break;
    case 0x33: // R-Type instrs
        exec_r_type(instr);
        break;
    case 0x23: // S-Type instrs
        exec_s_type(instr);
        break;
    case 0x37: // LUI
    case 0x17: // AUIPC
        exec_u_type(instr);
        break;
    case 0x63: // B 型指令
        exec_b_type(instr);
        break;
    case 0x6F: // JAL
        exec_j_type(instr);
        break;
    default:
        printf("Unknown instruction");
    }
    printf("\n");
    // printf("%04x %s  \t# x[%d]=%d\n", pc, asm1, rd, reg[rd]);
}
*/

