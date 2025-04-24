#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

extern void list_print(list *this, void (*print_value)(void *));
void print_int(void *arg);

int main(void)
{
        //////////////////////////////////////////////////
        /// list_new
        //////////////////////////////////////////////////
        printf("> new list\n");
        list *l_int = list_new(int);
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_push_front
        //////////////////////////////////////////////////
        printf("> push in head of the list\n");
        for (int i = 0; i < 3; i++) {
                list_insert_first(l_int, &(int){i});
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_pop_back
        //////////////////////////////////////////////////
        printf("> pop from tail of the list\n");
        for (int i = 0; i < 3; i++) {
                free(list_remove_last(l_int));
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_push_back
        //////////////////////////////////////////////////
        printf("> push in tail of the list\n");
        for (int i = 0; i < 5; i++) {
                list_insert_last(l_int, &(int){i + 1});
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_pop_front
        //////////////////////////////////////////////////
        printf("> pop from head of the list\n");
        for (int i = 0; i < 5; i++) {
                free(list_remove_first(l_int));
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_add
        //////////////////////////////////////////////////
        printf("> add to the list\n");
        for (int i = 0; i < 6; i++) {
                list_insert_at(l_int, &(int){i + 1}, i / 2);
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_remove
        //////////////////////////////////////////////////
        printf("> remove from the list\n");
        for (int i = 6; i > 0; i--) {
                list_remove_at(l_int, i / 2);
        }
        list_print(l_int, print_int);
        //////////////////////////////////////////////////
        /// list_delete
        //////////////////////////////////////////////////
        printf("> delete list\n");
        list_delete(l_int);
        return 0;
}

void print_int(void *arg) { printf("%04d", *(int *)arg); }
