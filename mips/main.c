#include "parser.h"
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void print_help(char* name)
{
    printf("usage: %s <binary> [param]\n", name);
    printf("param:\n");
    printf("    -h : show help message\n");
    printf("    -l : decompile it as a little endian file (default)\n");
    printf("    -b : decompile it as a big endian file\n");
}

int main(int argc, char** argv)
{
    if(argc == 1)
        errx(1, "usage: %s <binary> [parameter]\n ./%s -h for help", argv[0], argv[0]);
    
    int bigend = 0;

    int k = 1;

    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(k == i)
                k++;
            for(int j = 1; argv[i][j]; j++)
            {
                switch(argv[i][j])
                {
                    case 'b':
                        bigend = 1;
                        break;
                    case 'l':
                        bigend = 0;
                        break;
                    case 'h':
                        print_help(argv[0]);
                        return 0;
                }
            }

        }
    }

    struct stat sb;

    if(stat(argv[1], &sb) == -1)
        errx(1, "stat failed");
    

    int file = open(argv[1], O_RDONLY);
    struct assembler as = {
        calloc(sb.st_size, 1),
        sb.st_size,
        bigend == 1 ? BENDIAN : LENDIAN
    };

    read(file, as.data, as.len);
    

    printf("len: %zu\n", as.len);
    parse(&as);

    free(as.data);
    close(file);
    
    return 0;
}
