#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "lib.h"

void do_elf_sections(FILE *file, Elf32_Ehdr *elf_header)
{
    Elf32_Shdr section_header;
    char *section_names;

    // 讀取段表名稱字串表
    fseek(file, elf_header->e_shoff + elf_header->e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&section_header, sizeof(Elf32_Shdr), 1, file);
    section_names = malloc(section_header.sh_size);
    fseek(file, section_header.sh_offset, SEEK_SET);
    fread(section_names, section_header.sh_size, 1, file);

    // 逐一讀取並解析段表
    for (int i = 0; i < elf_header->e_shnum; i++)
    {
        fseek(file, elf_header->e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&section_header, sizeof(Elf32_Shdr), 1, file);

        char *section_name = &section_names[section_header.sh_name];
        char *section_body = malloc(section_header.sh_size);
        fseek(file, section_header.sh_offset, SEEK_SET);
        fread(section_body, 1, section_header.sh_size, file);

        do_elf_section(section_name, section_body, &section_header);

        if (strcmp(section_name, ".text") == 0)
        {
            do_text_section(section_body, &section_header);
        }
        else if (strcmp(section_name, ".symtab") == 0)
        {
            Elf32_Sym *symbols = (Elf32_Sym*) section_body;

            Elf32_Shdr strtab_header;
            // 找到符號表的字串表
            fseek(file, elf_header->e_shoff + section_header.sh_link * sizeof(Elf32_Shdr), SEEK_SET);
            fread(&strtab_header, sizeof(Elf32_Shdr), 1, file);

            // 讀取字串表
            char *strtab = malloc(strtab_header.sh_size);
            fseek(file, strtab_header.sh_offset, SEEK_SET);
            fread(strtab, strtab_header.sh_size, 1, file);

            do_sym_section(section_body, &section_header, symbols, strtab);

            free(symbols);
            free(strtab);
        }
        else if (strcmp(section_name, ".strtab") == 0 || strcmp(section_name, ".shstrtab") == 0)
        {
            do_str_tab(section_body, &section_header);
        }
    }
    free(section_names);
}

void do_elf(char *elf_file_name) {
    FILE *file = fopen(elf_file_name, "rb");
    if (!file)
    {
        perror("無法開啟檔案");
    }

    Elf32_Ehdr elf_header;
    fread(&elf_header, 1, sizeof(Elf32_Ehdr), file);
    // 檢查 ELF 檔頭魔數 (magic number)
    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0)
    {
        error("ELFMAG=%s e_ident=%s SELFMAG=%d\n", ELFMAG, elf_header.e_ident, SELFMAG);
        error("這不是一個有效的 ELF 檔案。\n");
        // exit(1);
    }
    do_elf_header(&elf_header);

    do_elf_sections(file, &elf_header);

    fclose(file);
}
