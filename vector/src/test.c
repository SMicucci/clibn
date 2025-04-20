#include "../include/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

enum vp_mode { NONE, LABEL, DATA, ALL };
static inline void print_vector(vector const *src, void (*print_val)(void *));
static inline void print_hex(void *arg, u_int64_t size);
static inline void print_int(void *arg);

int main(void)
{
        //////////////////////////////////////////////////
        /// init vector
        //////////////////////////////////////////////////
        vector *v_int = vector_new(int);
        printf("> initialized vector\n");
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// push and pop stuff
        //////////////////////////////////////////////////
        int n_push = 20;
        printf("[ ");
        for (int i = 0; i < n_push; i++) {
                vector_push(v_int, &(int){3 + (i << 2)});
                printf("%d", 3 + (i << 2));
                if (n_push - i - 1) {
                        printf(", ");
                }
        }
        printf(" ] pushed v_int\n");
        print_vector(v_int, print_int);
        // time to pop
        printf("[ ");
        for (int i = 0; i < n_push / 2; i++) {
                int *r = vector_pop(v_int);
                int v = *r;
                printf("%d", v);
                if (n_push / 2 - i - 1) {
                        printf(", ");
                }
                free(r);
        }
        printf(" ] popped v_int\n");
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// free vector
        //////////////////////////////////////////////////
        vector_delete(v_int);
        printf("> freed vector\n");
        return 0;
}

#define D "\x1b[0m"
#define S "\x1b[1m"
#define s "\x1b[22m"
#define I "\x1b[3m"
#define X "\x1b[37m"
#define R "\x1b[31m"
#define G "\x1b[32m"
#define Y "\x1b[33m"
#define B "\x1b[34m"
#define P "\x1b[35m"
#define C "\x1b[36m"

static inline void print_vector(vector const *src, void (*print_value)(void *))
{
        u_int64_t n = vector_nelem(src);
        printf("%svector%s<%s%s%s>%s%s[%s%lu%s]%s: %s%lu%s\n", C, Y, B,
               vector_type(src), Y, X, S, R, vector_cap(src), X, D, S, n, s);
        printf("%sdata%s: %s[%s ", C, D, S, P);
        for (u_int64_t i = 0; i < n; i++) {
                void *val = vector_get(src, i);
                if (print_value) {
                        print_value(val);
                } else {
                        print_hex(val, vector_size(src));
                }
                free(val);
                if (n - i - 1) {
                        printf("%s, %s", X, P);
                }
        }
        printf("%s]%s\n", X, D);
        return;
}

#undef D
#undef S
#undef s
#undef I
#undef X
#undef R
#undef G
#undef Y
#undef B
#undef P
#undef C

static inline void print_int(void *arg)
{
        printf("%04d", *(int *)arg);
        return;
}

static inline void print_hex(void *arg, u_int64_t size)
{
        for (u_int64_t i = 0; i < size; i++) {
                printf("%02X-", ((unsigned char *)arg)[i]);
        }
        return;
}
