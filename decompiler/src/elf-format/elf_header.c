#include "elf-format/elf_header.h"

#include <stdio.h>

headers* parse(void* data)
{
    elf_header_top* tmp = (elf_header_top*)data;

    switch (tmp->e_ident)
    {
        1
    }
}

void debug_elf_header_top(elf_header_top *header)
{
    printf("ei_ma:");
    for(int i = 0; i < 4; i++) printf(" %hhx",header->ei_ma[i]);
    printf("\n");

    printf("ei_class: %hhx\n", header->ei_class);
    printf("ei_data: %hhx\n", header->ei_data);
    printf("ei_version: %hhx\n", header->ei_version);
    printf("ei_osabi: %hhx\n", header->ei_osabi);
    printf("ei_abiversion: %hhx\n", header->ei_abiversion);
    printf("padding:");
    for(int i = 0; i < 7; i++) printf(" %hhx", header->ei_pad[i]);
    printf("\n");
    
    printf("e_type: %hx\n", header->e_type);
    printf("e_machine: %hx\n", header->e_machine);

    printf("e_version: %hx\n", header->e_version);

    return;
}

void debug_elf32_header(elf_header_32* header)
{
    debug_elf_header_top((elf_header_top*)header);

    printf("e_entry: %x\n", header->e_entry);
    printf("e_phoff: %x\n", header->e_phoff);
    printf("e_shoff: %x\n", header->e_shoff);

    printf("e_flags: %x\n",  header->e_flags);
    printf("e_ehsize: %hx\n", header->e_ehsize);     
    printf("e_phentsize: %hx\n", header->e_phentsize); 
    printf("e_phnum: %hx\n", header->e_phnum);
    printf("e_shentsize: %hx\n", header->e_shentsize);
    printf("e_shnum: %hx\n", header->e_shnum);
    printf("e_shstrndx: %hx\n", header->e_shstrndx);
}

void debug_elf64_header(elf_header_64* header)
{
    debug_elf_header_top((elf_header_top*)header);

    printf("e_entry: %lx\n", header->e_entry);
    printf("e_phoff: %lx\n", header->e_phoff);
    printf("e_shoff: %lx\n", header->e_shoff);

    printf("e_flags: %x\n",  header->e_flags);
    printf("e_ehsize: %hx\n", header->e_ehsize);     
    printf("e_phentsize: %hx\n", header->e_phentsize); 
    printf("e_phnum: %hx\n", header->e_phnum);
    printf("e_shentsize: %hx\n", header->e_shentsize);
    printf("e_shnum: %hx\n", header->e_shnum);
    printf("e_shstrndx: %hx\n", header->e_shstrndx);
}
