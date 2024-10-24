

*

## 

```c
typedef struct elf32_shdr {
  Elf32_Word	sh_name;      // 節點名稱，這是一個索引，指向節名稱表 (.shstrtab) 的某個位置。
  Elf32_Word	sh_type;      // 節點類型，定義了該節的內容類型，例如 SHT_PROGBITS, SHT_SYMTAB 等。
  Elf32_Word	sh_flags;     // 節點標誌，描述該節的屬性，可能的標誌有 SHF_WRITE（可寫）、SHF_ALLOC（載入到記憶體中）等。
  Elf32_Addr	sh_addr;      // 節點載入時的記憶體地址，執行時該節會被載入到這個地址。如果該節沒有被載入，則為 0。
  Elf32_Off	sh_offset;    // 節點在檔案中的偏移量，指出該節在 ELF 檔案中的位置，以檔案開頭為基準。
  Elf32_Word	sh_size;      // 節點大小，表示該節的大小（以字節為單位）。
  Elf32_Word	sh_link;      // 與該節關聯的其他節的索引。具體含義取決於節類型。例如，符號表 (.symtab) 的節點可能會指向字符串表 (.strtab)。
  Elf32_Word	sh_info;      // 附加資訊，含義依節類型而異。對於符號表來說，這是局部符號的索引
```