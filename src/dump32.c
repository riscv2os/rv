#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <elf.h>

#include "lib.h"
#include "dasm.h"

void do_elf_header(Elf32_Ehdr *elf_header)
{
    printf("ELF 類型: %d\n", elf_header->e_type);
    printf("機器類型: %d\n", elf_header->e_machine);
    printf("進入點位址: 0x%x\n", elf_header->e_entry);
    printf("段表偏移量: %d\n", elf_header->e_shoff);
    printf("程式表偏移量: %d\n", elf_header->e_phoff);
}

void do_elf_section(char *section_name, char *section_body, Elf32_Shdr *section_header) {
    printf("段名稱: %-20s ", section_name);
    printf("段位址: 0x%08x ", section_header->sh_addr);
    printf("段大小: %8d\n", section_header->sh_size);
    printf("整段內容印出:\n");
    for (uint32_t j = 0; j < section_header->sh_size; j++)
    {
        printf("%02x ", (unsigned char)section_body[j]);
        if ((j + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n\n");
}

void do_text_section(char *section_body, Elf32_Shdr *section_header) {
    printf(".text 段反組譯結果:\n");
    disassemble_block(section_body, section_header->sh_size);
    printf("\n\n");
}

void do_rodata_section(char *section_body, Elf32_Shdr *section_header) {
    printf(".rodata 段解讀: ....\n");
    for (uint32_t j = 0; j < section_header->sh_size; j++)
    {
        char ch = section_body[j];
        printf("%c", ch == '\0' ? '/' : ch);
    }
    printf("\n\n");
}

void do_sbss_section(char *section_body, Elf32_Shdr *section_header) {
    printf(".sbss 段解讀:...\n");
    printf("\n\n");
}

void do_sym_section(char *section_body, Elf32_Shdr *section_header, Elf32_Sym *symbols, char *strtab) {
    // 如果是符號表，則讀取並顯示符號
    printf("符號表:\n");
    int num_symbols = section_header->sh_size / sizeof(Elf32_Sym);
    for (int j = 0; j < num_symbols; j++)
    {
        printf("符號: %s, 位址: 0x%08x\n",
                &strtab[symbols[j].st_name], symbols[j].st_value);
        if (strcmp(&strtab[symbols[j].st_name], "main") == 0)
            printf("   ==> main 的位址在 0x%08x\n", symbols[j].st_value);
    }
    printf("\n\n");
}

void do_str_tab(char *section_body, Elf32_Shdr *section_header) {
    printf("字串表:\n");
    for (uint32_t j = 0; j < section_header->sh_size; j++)
    {
        char ch = section_body[j];
        printf("%c", ch == '\0' ? '/' : ch);
    }
    printf("\n\n");
}

#include "elf32do.c"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("使用方式: %s <ELF 檔案>\n", argv[0]);
        return 1;
    }

    do_elf(argv[1]);
    return 0;
}
