#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>
#include <stdbool.h>

#include "riscv.h"
#include "lib.h"
#include "dasm.h"
#include "vm.h"

char *code_body;
int code_entry;
int code_size;

static void do_elf_header(Elf32_Ehdr *elf_header) {}

static void do_elf_section(char *section_name, char *section_body, Elf32_Shdr *section_header) {
}

static void do_text_section(char *section_body, Elf32_Shdr *section_header) {
    code_body = section_body;
    code_size = section_header->sh_size;
}

static void do_sym_section(char *section_body, Elf32_Shdr *section_header, Elf32_Sym *symbols, char *strtab) {
    // 如果是符號表，則讀取並顯示符號
    printf("符號表:\n");
    int num_symbols = section_header->sh_size / sizeof(Elf32_Sym);
    for (int j = 0; j < num_symbols; j++)
    {
        printf("符號: %s, 位址: 0x%08x\n",
                &strtab[symbols[j].st_name], symbols[j].st_value);
        if (strcmp(&strtab[symbols[j].st_name], "main") == 0) {
            code_entry = symbols[j].st_value;
            printf("   ==> main 的位址在 0x%08x\n", symbols[j].st_value);
        }
    }
    printf("\n\n");
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
    vm_run(code_body, code_size, code_entry);
    /*
    FILE *file = fopen(argv[1], "rb");
    if (!file)
    {
        perror("無法開啟檔案");
        return 1;
    }

    Elf32_Ehdr elf_header;
    fread(&elf_header, 1, sizeof(Elf32_Ehdr), file);

    // dump_elf_header(file, elf_header);
    // dump_elf_sections(file, elf_header);
    block_t code_block;
    load_into_memory(file, elf_header, &code_block);
    printf("entry=%04x\n", code_block.entry);
    // disassemble_block(code_block.body, code_block.size);
    vm_run(code_block.body, code_block.size, code_block.entry);
    fclose(file);
    */
    return 0;
}


/*
void load_into_memory(FILE *file, Elf32_Ehdr elf_header, block_t *code_block) {
    Elf32_Shdr section_header;
    char *section_names;

    // 讀取段表名稱字串表
    fseek(file, elf_header.e_shoff + elf_header.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&section_header, sizeof(Elf32_Shdr), 1, file);
    section_names = malloc(section_header.sh_size);
    fseek(file, section_header.sh_offset, SEEK_SET);
    fread(section_names, section_header.sh_size, 1, file);

    // 逐一讀取並解析段表
    for (int i = 0; i < elf_header.e_shnum; i++) {
        fseek(file, elf_header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&section_header, sizeof(Elf32_Shdr), 1, file);
        char *section_name = &section_names[section_header.sh_name];
        if (strcmp(section_name, ".text")==0) {
            code_block->size = section_header.sh_size;
            code_block->body = malloc(section_header.sh_size);
            fseek(file, section_header.sh_offset, SEEK_SET);
            fread(code_block->body, 1, section_header.sh_size, file);
        } else if (strcmp(section_name, ".symtab") == 0) {
            Elf32_Shdr strtab_header;
            // 找到符號表的字串表
            fseek(file, elf_header.e_shoff + section_header.sh_link * sizeof(Elf32_Shdr), SEEK_SET);
            fread(&strtab_header, sizeof(Elf32_Shdr), 1, file);

            // 讀取符號表
            Elf32_Sym *symbols = malloc(section_header.sh_size);
            fseek(file, section_header.sh_offset, SEEK_SET);
            fread(symbols, section_header.sh_size, 1, file);
            
            // 讀取字串表
            char *strtab = malloc(strtab_header.sh_size);
            fseek(file, strtab_header.sh_offset, SEEK_SET);
            fread(strtab, strtab_header.sh_size, 1, file);

            int num_symbols = section_header.sh_size / sizeof(Elf32_Sym);
            for (int j = 0; j < num_symbols; j++) {
                if (strcmp(&strtab[symbols[j].st_name], "main")==0) {
                    printf("==> main 的位址在 0x%08x\n", symbols[j].st_value);
                    code_block->entry = symbols[j].st_value;
                }
            }

            free(symbols);
            free(strtab);
        }
    }
    free(section_names);
}
*/
