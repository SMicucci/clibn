#include "../include/iterator.h"
#include <stdio.h>
#include <stdlib.h>

void array_dtor(iterator *this);
void array_set(iterator *this, void *value);
void *array_get(iterator *this);
iterator *array_next(iterator *this);
iterator *array_prev(iterator *this);
iterator *array_get_iterator(int *array, int size);

typedef struct {
        iterator super;
        int position;
        int capacity;
        int *data;
} array_iterator;

int main(void)
{
        printf("> create an iterable array\n");
        int size = 10;
        int *iterable = calloc(size, sizeof(*iterable));
        int *val = NULL;
        iterator *iter = array_get_iterator(iterable, size);
        ////////////////////////////////////////////////////////////////////////
        /// iterator_get
        ////////////////////////////////////////////////////////////////////////
        printf("> get value from iterator\n");
        val = iterator_get(iter);
        printf("value => %d\n", *val);
        int counter = *val;
        ////////////////////////////////////////////////////////////////////////
        /// iterator_next
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        /// iterator_set
        ////////////////////////////////////////////////////////////////////////
        printf("> read and set next iterator\n");
        while (1) {
                if (!iterator_next(iter))
                        break;
                val = iterator_get(iter);
                printf("next => %d\n", *val);
                counter++;
                iterator_set(iter, &(counter));
        }
        ////////////////////////////////////////////////////////////////////////
        /// iterator_prev
        ////////////////////////////////////////////////////////////////////////
        printf("> get prev iterator\n");
        while (1) {
                if (!iterator_prev(iter))
                        break;
                val = iterator_get(iter);
                printf("prev => %d\n", *val);
        }
        printf("> free iterator\n");
        printf("iterable => [");
        for (int i = 0; i < size; i++) {
                printf("%d, ", iterable[i]);
        }
        printf("\x1b[1D\x1b[1D]\n");
        iterator_destroy(iter);
        return 0;
}

// implementation of the required method

void array_dtor(iterator *this)
{
        array_iterator *arr = (array_iterator *)this;
        free(arr->data);
        free(arr);
}

void array_set(iterator *this, void *value)
{
        array_iterator *arr = (array_iterator *)this;
        arr->data[arr->position] = *(int *)value;
}

void *array_get(iterator *this)
{
        array_iterator *arr = (array_iterator *)this;
        return &arr->data[arr->position];
}

iterator *array_next(iterator *this)
{
        array_iterator *arr = (array_iterator *)this;
        arr->position++;
        if (arr->position == arr->capacity)
                return NULL;
        return this;
}

iterator *array_prev(iterator *this)
{
        array_iterator *arr = (array_iterator *)this;
        arr->position--;
        if (arr->position < 0)
                return NULL;
        return this;
}

iterator *array_get_iterator(int *array, int size)
{
        array_iterator *res = calloc(1, sizeof(*res));
        res->capacity = size;
        res->position = 0;
        res->data = array;
        iterator_create((iterator *)res, array_get, array_set, array_next,
                        array_prev, array_dtor);
        return (iterator *)res;
}
