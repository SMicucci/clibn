#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

extern void list_print(list *this, void (*print_value)(void *));
void print_int(void *arg);

#define TEST_SIZE 20
#define MAGIC_NUMBER 420

int main(void)
{
        int *value = calloc(1, sizeof(*value));
        ////////////////////////////////////////////////////////////////////////
        /// list_new
        ////////////////////////////////////////////////////////////////////////
        printf("> new list\n");
        list *l_int = list_new(int);
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_insert_first
        ////////////////////////////////////////////////////////////////////////
        printf("> insert in front of the list\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                list_insert_first(l_int, &(int){i});
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_set_first - list_peek_first
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the list\n");
        *value = MAGIC_NUMBER;
        list_set_first(l_int, value);
        value = list_peek_first(l_int);
        printf(">>> after change: %d\n", *value);
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_remove_first
        ////////////////////////////////////////////////////////////////////////
        printf("> remove at front of the list\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                free(list_remove_first(l_int));
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_insert_last
        ////////////////////////////////////////////////////////////////////////
        printf("> insert at back of the list\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                list_insert_last(l_int, &(int){i + 1});
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_set_last - list_peek_last
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the list\n");
        *value = MAGIC_NUMBER;
        list_set_last(l_int, value);
        value = list_peek_last(l_int);
        printf(">>> after change: %d\n", *value);
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_remove_last
        ////////////////////////////////////////////////////////////////////////
        printf("> pop from tail of the list\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                free(list_remove_last(l_int));
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_insert_at
        ////////////////////////////////////////////////////////////////////////
        printf("> insert arbitrarly into the list\n");
        for (int i = 0; i < TEST_SIZE; i++) {
                list_insert_at(l_int, &(int){i + 1}, i / 2);
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_set_at - list_peek_at
        ////////////////////////////////////////////////////////////////////////
        printf("> set and peek in fron of the list\n");
        *value = MAGIC_NUMBER;
        list_set_at(l_int, value, TEST_SIZE / 2);
        value = list_peek_at(l_int, TEST_SIZE / 2);
        printf(">>> after change: %d\n", *value);
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_remove_at
        ////////////////////////////////////////////////////////////////////////
        printf("> remove arbitrarly from the list\n");
        for (int i = TEST_SIZE; i > 0; i--) {
                list_remove_at(l_int, i / 2);
        }
        list_print(l_int, print_int);

        ////////////////////////////////////////////////////////////////////////
        /// list_delete
        ////////////////////////////////////////////////////////////////////////
        printf("> delete list\n");
        list_delete(l_int);
        free(value);
        return 0;
}

void print_int(void *arg) { printf("%04d", *(int *)arg); }
