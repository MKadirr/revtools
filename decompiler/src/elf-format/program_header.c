#include "elf-format/elf_program_header.h"
#include <stdio.h>

void debug_program_header_elf32(program_header_elf32* header)
{
    printf("Program header:\n");
    printf("p_type:   %x\n", header->p_type);
    printf("p_offset: %x\n", header->p_offset);
    printf("p_vaddr:  %x\n", header->p_vaddr);
    printf("p_paddr:  %x\n",  header->p_paddr);
    printf("p_filesz: %x\n", header->p_filesz);     
    printf("p_memsz:  %x\n", header->p_memsz); 
    printf("p_flags:  %x\n", header->p_flags);
    printf("p_align:  %x\n", header->p_align);
}

void debug_program_header_elf64(program_header_elf64* header)
{
    printf("Program header:\n");
    printf("p_type:   %x\n", header->p_type);
    printf("p_flags:  %x\n", header->p_flags);
    printf("p_offset: %lx\n", header->p_offset);
    printf("p_vaddr:  %lx\n", header->p_vaddr);
    printf("p_paddr:  %lx\n",  header->p_paddr);
    printf("p_filesz: %lx\n", header->p_filesz);
    printf("p_memsz:  %lx\n", header->p_memsz);
    printf("p_align:  %x\n", header->p_align);
}


