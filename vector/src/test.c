#include "../include/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

extern void vector_print(vector *this, void (*print_value)(void *));
void print_int(void *arg);

#define TEST_SIZE 20
#define MAGIC_NUMBER 420

int main(void)
{
        int *value = calloc(1, sizeof(*value));
        ////////////////////////////////////////////////////////////////////////
        /// vector_new
        ////////////////////////////////////////////////////////////////////////
        printf("> new vector\n");
        vector *l_int = vector_new(int);
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_insert_first
        ////////////////////////////////////////////////////////////////////////
        printf("> insert in front of the vector\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                vector_insert_first(l_int, &(int){i});
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_set_first - vector_peek_first
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the vector\n");
        *value = MAGIC_NUMBER;
        vector_set_first(l_int, value);
        free(value);
        value = vector_peek_first(l_int);
        printf(">>> after change: %d\n", *value);
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_remove_first
        ////////////////////////////////////////////////////////////////////////
        printf("> remove at front of the vector\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                free(vector_remove_first(l_int));
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_insert_last
        ////////////////////////////////////////////////////////////////////////
        printf("> insert at back of the vector\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                vector_insert_last(l_int, &(int){i + 1});
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_set_last - vector_peek_last
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the vector\n");
        *value = MAGIC_NUMBER;
        vector_set_last(l_int, value);
        free(value);
        value = vector_peek_last(l_int);
        printf(">>> after change: %d\n", *value);
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_remove_last
        ////////////////////////////////////////////////////////////////////////
        printf("> pop from tail of the vector\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                free(vector_remove_last(l_int));
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_insert_at
        ////////////////////////////////////////////////////////////////////////
        printf("> insert arbitrarly into the vector\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                vector_insert_at(l_int, &(int){i + 1}, i / 2);
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_set_at - vector_peek_at
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the vector\n");
        *value = MAGIC_NUMBER;
        vector_set_at(l_int, value, TEST_SIZE / 2);
        free(value);
        value = vector_peek_at(l_int, TEST_SIZE / 2);
        printf(">>> after change: %d\n", *value);
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_remove_at
        ////////////////////////////////////////////////////////////////////////
        printf("> remove arbitrarly from the vector\n");
        for (int i = TEST_SIZE; i > 0; i--) {
                free(vector_remove_at(l_int, i / 2));
        }
        vector_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// vector_delete
        ////////////////////////////////////////////////////////////////////////
        printf("> delete vector\n");
        vector_delete(l_int);
        free(value);
        return 0;
}

void print_int(void *arg) { printf("%04d", *(int *)arg); }
