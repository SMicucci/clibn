#include "../include/list.h"
#include <stdio.h>

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
        printf("> push in front to list\n");
        for (int i = 0; i < 5; i++) {
                list_push_front(l_int, &(int){i + 1});
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
