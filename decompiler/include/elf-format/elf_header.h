#ifndef ELF_HEADER_H
#define ELF_HEADER_H

#define ET_NONE   0x0000 //Unknown
#define ET_RE     0x0001 //Relocatable file
#define ET_exec   0x0002 //executable file
#define ET_DYN    0x0003 //shared object
#define ET_CORE   0x0004 // core file

#define ET_LOOS   0xFE00 // | reserved inclusive range.
#define ET_HIOS   0xFEFF // | operating system specific.

#define ET_LOPROC 0xFF00 // | Rersved inclusive range.
#define ET_HIPROC 0xFFFF // | Processor specific.

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

typedef struct Elf_header_top {
    u8 ei_ma[4];
    u8 ei_class;
    u8 ei_data;
    u8 ei_version;
    u8 ei_osabi;
    u8 ei_abiversion;
    u8 ei_pad[7];
    
    u16 e_type;
    u16 e_machine;

    u16 e_version;
} elf_header_top;

typedef struct Elf_header_32 {
    u8 ei_ma[4];
    u8 ei_class;
    u8 ei_data;
    u8 ei_version;
    u8 ei_osabi;
    u8 ei_abiversion;
    u8 ei_pad[7];
    
    u16 e_type;
    u16 e_machine;

    u16 e_version;

    u32 e_entry; // program entry point
    u32 e_phoff; // point to start of program header (usually 0x34 or 0x40 for x32 or x64)
    u32 e_shoff; // point to start of section header table

    u32 e_flags;
    u16 e_ehsize; // header size
    u16 e_phentsize; // size of program header table
    u16 e_phnum; // nb of entries in the program header table
    u16 e_shentsize; // size of section header table
    u16 e_shnum; // number of entry in the 
    u16 e_shstrndx; //index of the section headeder table entry that contains 
                    //the section name64

} elf_header_32;

typedef struct Elf_header_64 {
    u8 ei_ma[4];
    u8 ei_class;
    u8 ei_data;
    u8 ei_version;
    u8 ei_osabi;
    u8 ei_abiversion;
    u8 ei_pad[7];
    
    u16 e_type;
    u16 e_machine;

    u16 e_version;

    u64 e_entry; // program entry point
    u64 e_phoff; // point to start of program header (usually 0x34 or 0x40 for x32 or x64)
    u64 e_shoff; // point to start of section header table

    u32 e_flags;
    u16 e_ehsize; // header size
    u16 e_phentsize; // size of program header table
    u16 e_phnum; // nb of entries in the program header table
    u16 e_shentsize; // size of section header table
    u16 e_shnum; // number of entry in the 
    u16 e_shstrndx; //index of the section headeder table entry that contains 
                    //the section name64
} elf_header_64;

#endif
