#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>
#include <stdbool.h>
#include <assert.h>

#include "riscv.h"
#include "lib.h"
#include "dasm.h"
#include "vm.h"

// char *memory;
#define MEMORY_SIZE 100000

char memory[MEMORY_SIZE];
int code_entry = -1;

static void do_elf_header(Elf32_Ehdr *elf_header)
{
    code_entry = elf_header->e_entry;
}

static void do_elf_section(char *section_name, char *section_body, Elf32_Shdr *section_header)
{
}

static void do_text_section(char *section_body, Elf32_Shdr *section_header)
{
    assert(section_header->sh_addr + section_header->sh_size < MEMORY_SIZE);
    memcpy(&memory[section_header->sh_addr], section_body, section_header->sh_size);
    printf(".rodata :起始位址 0x%x 大小 0x%x\n內容\n", section_header->sh_addr, section_header->sh_size);
    disassemble_block(section_body, section_header->sh_size);
    printf("\n\n");
}

void do_rodata_section(char *section_body, Elf32_Shdr *section_header)
{
    assert(section_header->sh_addr + section_header->sh_size < MEMORY_SIZE);
    memcpy(&memory[section_header->sh_addr], section_body, section_header->sh_size);
    printf(".rodata :起始位址 0x%x 大小 0x%x\n內容\n", section_header->sh_addr, section_header->sh_size);
    dump_elf_stab(section_body, section_header->sh_size);
    printf("\n\n");
}

void do_sbss_section(char *section_body, Elf32_Shdr *section_header)
{
    assert(section_header->sh_addr + section_header->sh_size < MEMORY_SIZE);
    memset(&memory[section_header->sh_addr], 0, section_header->sh_size);
    printf(".sbss :起始位址 0x%x 大小 0x%x\n\n", section_header->sh_addr, section_header->sh_size);
}

static void do_sym_section(char *section_body, Elf32_Shdr *section_header, Elf32_Sym *symbols, char *strtab)
{
}

static void do_str_tab(char *section_body, Elf32_Shdr *section_header) {}

#include "elf32do.c"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("使用方式: %s <ELF 檔案>\n", argv[0]);
        return 1;
    }

    do_elf(argv[1]);
    // vm_run(memory, MEMORY_SIZE, code_entry);
    return 0;
}
