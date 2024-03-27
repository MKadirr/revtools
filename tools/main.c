#include <stdio.h>
#include <sys/mman.h>
#include "utils.h"
#include <string.h>

int main()
{
    struct s_long *a = to_hexa("50 b3 67 af a5 0e 77 a3 4a a2 9b 01 7d 89 61 a5 a5 02 76 b2 70 b8 89 03 79 b8 71 95 9b 28 74 bf 61 be 96 12 47 95 3e e1 a5 04 6c a3 73 ac 89 00");
   
    char b[] = {0x18, 0xd6, 0x15, 0xca, 0xfa, 0x77};

    print_slong(a, 1);

    char* d = a->a;

    for(int i = 0; d[i]; i++)
    {
        printf("%hhx ^ %hhx = '%c'\n", d[i], b[i % 6], d[i] ^ b[i % 6]);
        d[i] = b[i % 6] + d[i];
    }

    printf("%s\n", "Here_you_have_to_understand_a_little_C++_stuffs");

    free_slong(a);

    return 0;
}
