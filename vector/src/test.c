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
        printf("> initialized vector \x1b[1m[%p]\x1b[0m\n", v_int);
        printf("  > test utilities with print vector function\n");
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// push stuff
        //////////////////////////////////////////////////
        int iter = 20;
        printf("\n> push %d value\n", iter);
        for (int i = 0; i < iter; i++) {
                vector_insert_last(v_int, &(int){3 + (i << 2)});
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// pop half size
        //////////////////////////////////////////////////
        iter >>= 1;
        printf("\n> pop %d value\n", iter);
        for (int i = 0; i < iter; i++) {
                free(vector_remove_last(v_int));
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// set odd value to zero
        //////////////////////////////////////////////////
        printf("\n> set to \"0xDEADBEEF\" odd value\n");
        for (int i = 0; i < (int)vector_len(v_int); i++) {
                if ((i % 2)) {
                        vector_set_at(v_int, &(int){0xDEADBEEF}, i);
                }
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// remove odd value
        //////////////////////////////////////////////////
        printf("\n> remove odd value\n");
        for (int i = (int)vector_len(v_int); i >= 0; i--) {
                if (i % 2) {
                        free(vector_remove_at(v_int, i));
                }
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// insert new fun value
        //////////////////////////////////////////////////
        printf("\n> insert new value \x1b[3m(NICE)\x1b[0m\n");
        for (int i = 0; i <= (int)vector_len(v_int); i++) {
                if (i % 2) {
                        vector_insert_at(v_int, &(int){69420}, i);
                }
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// clean vector
        //////////////////////////////////////////////////
        printf("\n> clean vector \x1b[3m(not required for free it)\x1b[0m\n");
        for (int i = vector_len(v_int); i > 0; i--) {
                free(vector_remove_last(v_int));
        }
        print_vector(v_int, print_int);
        //////////////////////////////////////////////////
        /// free vector
        //////////////////////////////////////////////////
        vector_delete(v_int);
        v_int = NULL;
        printf("\n> freed vector \x1b[1m[%p]\x1b[0m\n", v_int);
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
        u_int64_t n = vector_len(src);
        char *t = vector_type(src);
        printf("%svector%s<%s%s%s>%s%s[%s%lu%s]%s: %s%lu%s\n", C, Y, B, t, Y, X,
               S, R, vector_cap(src), X, D, S, n, s);
        free(t);
        printf("%sdata%s: %s[%s ", C, D, S, P);
        for (u_int64_t i = 0; i < n; i++) {
                void *val = vector_peek_at(src, i);
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
        printf("%2d", *(int *)arg);
        return;
}

static inline void print_hex(void *arg, u_int64_t size)
{
        for (u_int64_t i = 0; i < size; i++) {
                printf("%02X-", ((unsigned char *)arg)[i]);
        }
        return;
}
