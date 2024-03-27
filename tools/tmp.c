#include <stdio.h>
#include "utils.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    struct s_long *a = to_hexa("41 00 02 3c 38 80 99 8f 41 00 10 3c f8 0d 42 24 40 00 04 3c 60 0c 05 26 0c 0c 84 24 98 01 06 24 03 00 07 24 09 f8 20 03 10 00 a2 af 18 00 bc 8f ff ff 02 24 68 80 99 8f 02 08 07 24 21 20 00 00 98 01 05 24 03 00 06 24 10 00 a2 af 09 f8 20 03 14 00 a0 af 18 00 bc 8f 60 0c 05 26 4c 80 99 8f 21 20 40 00 98 01 06 24 09 f8 20 03 21 90 40 00 18 00 bc 8f 21 20 40 02 50 80 99 8f 98 01 05 24 09 f8 20 03 05 00 06 24 04 00 24 8e 21 c8 40 02 09 f8 20 03 25 08 20 00 18 00 bc 8f 25 08 20 00 44 80 99 8f 04 00 40 10 21 80 40 00 40 00 04 3c 03 00 00 10 18 0c 84 24");
    
    int file = open("test", 
            O_CREAT | 
            O_RDWR | 
            S_IRWXU
            );

    write(file, a->a, a->len);
    
    close(file);


    free_slong(a);

    return 0;
}
