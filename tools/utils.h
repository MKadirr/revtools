struct s_long
{
    size_t len;
    unsigned char *a;
};

void free_slong(struct s_long*);
void print_slong(struct s_long* sl, int flag);

struct s_long* to_hexa(const char* str);
