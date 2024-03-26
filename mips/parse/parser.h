#include <stddef.h>

#define BENDIAN    2
#define LENDIAN 1

typedef unsigned int uint;

struct assembler {
    unsigned char *data;
    size_t len;
    int endian;
};

void parse(struct assembler* file);
