#ifndef ELF_PROGRAM_HEADER
#define ELF_PROGRAM_HEADER

#include "elf_header.h"

typedef struct Program_header_elf32 {
    u32 p_type;
    u32 p_offset;
    u32 p_vaddr;
    u32 p_paddr;
    u32 p_filesz;
    u32 p_memsz;
    u32 p_flags;
    u32 p_align;
} program_header_elf32;

typedef struct Program_header_elf64 {
    u32 p_type;
    u32 p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u32 p_align;
} program_header_elf64;

#endif
