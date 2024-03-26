#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "elf-format/elf_header.h"

void* load(const char *path)
{
    struct stat statbuf;
    if(stat(path, &statbuf) == -1)
        err(1, "error stat");

    void* data = (unsigned char*)calloc(statbuf.st_size, 1);
    
    int file = open(path, 0);

    if(read(file, data, statbuf.st_size) == -1)
        err(1, "error read");

    close(file);

    return data;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        err(1, "usage: %s  <file>\n", argv[0]);
    }

    void* data = load((const char*)argv[1]);
    
    write(1, data, 4);

    free(data);
}
