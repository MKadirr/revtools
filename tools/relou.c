#include <stdio.h>
#include <sys/mman.h>
#include "utils.h"

int main()
{
    struct s_long *a = to_hexa("84 cc 0f b0 c2 c3 3b da 21 68 28 0f e7 76 a9 dc fd 8c 4a f8 49 7c 20 e7 8a 09 94 9f f1 a8 58 c5 6e 3d fb cc 59 e9 52 70 58 20 6e 5d f7 97 f0 47 9b 57 12 e0 67 1e f7 a8 e4 bf ec 5e 5f 9b ae b6 75 ac c7 4b ba 35 e7 51 a1 07 e2 07 76 d4 0a 53 ca 1d 38 8d db 02 7c e8 07 dc c0 25 f0 44 9f a4 40 91 46 fc bb 59 5b be dc 4f 80 17 eb ed 1f c0 6d 3d b0 4e 2b d5 29 5b b0 39 d4 2c 33 3a a0 4c ea 85 15 bf d8 15 0f e0 8a e1 10 54 bd 09 fa 70 39 da 90 4e af ee 62 2e 1c fa 49 3f e5 64 69 d3 a3 04 51 3c 60 e2 78 51 af 29 02 11 1d b7 40 f0 71 fb 2c 4c 76 1f a8 d2 4f 3c f1 84 6c 47 c1 09 17 2c c3 7a 7b ae 10 1d 88 24 c0 c2 a4 fa 67 57 1f 78 42 e5 a5 b4 ea d8 db 1e 26 2a 8f 65 e1 16 07 84 90 9a 1a e5 be 0d 6e 47 fd 18 32 b6 3f 93 d6 dc 52 1f a4 04 88 5d 0f fe 52 fe 35 9e 37 21 5a 95 15 54 1f 16 ab 87 70 b7 90 f8 98 fa cb 0e 24 0d c8 ae 78 dc 80 a5 b2 ac 0e dd 73 5d 90 c0 b0 1a 5d 61 62 4a e7 63 25 b0 22 66 cf 89 84 93 77 00 2c c1 73 de 48 90 4a 71 bc 52 29 35 2b 20 af bc 78 1a 5b 90 8d c1 a2 69 91 ea f8 37 f9 ac ae 82 fc 3f 0e 2a 09 90 c6 ed b4 3a b2 15 90 31 ea 8c 2f 82 41 bd fb dc d0 c4 2a b4 ef df d7 3f cb 87 36 71 6e cd 1b 04 8b 61 09 81 1e 46 04 74 5a 18 3f 43 77 89 38 00 93 4a 09 bc 8a 3a 25 35 0b 97 ef b0 1e 63 09 79 00");
    
    struct s_long *b = to_hexa("cb 38 db a4 a1 34 5a be");

    print_slong(a, 1);
    print_slong(b, 1);

    cbc_crypt("n0t_h4rd", a->a, 0x198, 3, b->a);
    
    print_slong(a, 1);
    
    free_slong(a);
    free_slong(b);

    return 0;
}
