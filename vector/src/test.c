#include "../include/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

enum vp_mode { NONE, LABEL, DATA, ALL };
static inline void print_vector(vector const *src, enum vp_mode mode);
static inline void print_bits(u_int8_t trg);

int main(void)
{
        // test vector creation
        void *v_int = vector_new(int);
        // add some stuff
        printf("> push some stuff...\n");
        vector_push(v_int, &((int){15 << 4}));
        vector_push(v_int, &((int){15 << 4}));
        vector_push(v_int, &((int){15 << 4}));
        print_vector(v_int, ALL); /* PRINT */
        // discrete insertion
        printf("> insert other stuff...\n");
        vector_insert(v_int, 0, &((int){15}));
        vector_insert(v_int, 2, &((int){15}));
        print_vector(v_int, DATA); /* PRINT */
        // discrete extraction
        printf("> remove newer stuff...\n");
        free(vector_remove(v_int, 0));
        free(vector_remove(v_int, 1));
        print_vector(v_int, DATA); /* PRINT */
        // wrong usage are safe?
        printf("> do some action out of scope stuff...\n");
        vector_insert(v_int, vector_count(v_int) + 2, &((int){15 << 4}));
        free(vector_remove(v_int, vector_count(v_int) + 2));
        print_vector(v_int, DATA); /* PRINT */
        // free it
        printf("> pop everything and beyond...\n");
        for (int i = 0; i < 10; i++) {
                free(vector_pop(v_int));
        }
        print_vector(v_int, ALL); /* PRINT */
        vector_delete(v_int);
        return EXIT_SUCCESS;
}

static inline void print_vector(vector const *src, enum vp_mode mode)
{
        if (mode & LABEL) {
                printf("vector<%s>[%lu]: %lu\n", vector_type(src),
                       vector_cap(src), vector_count(src));
        }
        if (mode & DATA) {
                u_int64_t n = vector_count(src);
                u_int64_t s = vector_size(src);
                if (n) {
                        printf("size: %lu\n - data:\n[\n", n);
                        for (u_int64_t i = 0; i < n; i++) {
                                u_int8_t *b_elem =
                                    (u_int8_t *)vector_get(src, i);
                                printf("%8lu:(", i);
                                for (u_int64_t j = 0; j < s; j++) {
                                        if (s > 8 && j > 2) {
                                                break;
                                        }
                                        print_bits(b_elem[j]);
                                        if (s - j - 1) {
                                                printf("-");
                                        }
                                }
                                if (s > 8) {
                                        printf("... ");
                                }
                                printf(")");
                                if (n - i - 1) {
                                        printf(",");
                                }
                                printf("\n");
                                free(b_elem);
                        }
                        printf("]\n");
                }
        }
        return;
}

static inline void print_bits(u_int8_t trg)
{
        for (int8_t i = 7; i >= 0; i--) {
                u_int8_t pos = 1 << i;
                u_int8_t mod = trg & pos;
                u_int8_t bit = mod ? 1 : 0;
                printf("%d", bit);
        }
}
