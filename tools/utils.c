#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void free_slong(struct s_long* sl)
{
    if(sl == NULL) return;
    free(sl->a);
    free(sl);
}

void print_slong(struct s_long* sl, int flag)
{
    if(sl == NULL) return;

    for(int i = 0; i < sl->len; i++)
    {
        if(flag & 2)
            printf("\\x");
        printf("%02hhx", sl->a[i]);
        if(flag & 1)
            printf(" ");
    }
    printf("\n");
}

struct s_long* to_hexa(const char* str)
{
    size_t i = 0;
    size_t len = 0;
    while(str[i] != '\x0')
    {
        if(str[i] != ' ')
            len++;
        i++;
    }

    

    struct s_long* r = calloc(1, sizeof(struct s_long));
    r->len = (len / 2) + (len % 2);
    r->a = calloc(r->len, sizeof(unsigned char));

    size_t k = len%2;

    i = 0;

    while(str[i] != 0)
    {
        if(str[i] != ' ')
        {
            if('0' <= str[i] && str[i] <= '9')
            {
                r->a[k/2] += (str[i] - '0') << (4 * ((k + 1) % 2));
            }
            else if('a' <= str[i] && str[i] <= 'f')
            {
                r->a[k/2] += (str[i] - 'a' + 0xa) << (4 * ((k + 1) % 2));
            }
            else if('A' <= str[i] && str[i] <= 'F')
            {
                r->a[k/2] += (str[i] - 'A' + 0xa) << (4 * ((k + 1) % 2));
            }
            else 
            {
                free_slong(r);
                return NULL;
            }
            k++;
        }
        i++;
    }

    return r;
}
