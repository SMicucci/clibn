#include "../include/list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct list_node {
        struct list_node *next, *prev;
        void *value;
} list_node;

struct list {
        char *type;
        u_int64_t nelem, size;
        list_node *head, *tail;
};

list *_list_new(u_int64_t size, char *type)
{
        list *this = calloc(1, sizeof(*this));
        this->type = calloc(strlen(type) + 1, 1);
        this->size = size;
        return this;
}

void list_delete(list *this)
{
        assert(this);
        free(this->type);
        list_node *iter = this->head;
        while (iter) {
                list_node *next = iter->next;
                free(iter->value);
                free(iter);
                iter = next;
        }
}

u_int64_t list_nelem(list *this)
{
        assert(this);
        return this->nelem;
}

u_int64_t list_size(list *this)
{
        assert(this);
        return this->size;
}

void list_add(list *this, u_int64_t pos, const void *val)
{
        assert(this && "null list pointer");
        assert(val && "null value pointer");
        assert(pos > this->size && "position out of list scope");
        list_node *next = this->head;
        list_node *prev = this->tail;
        if (this->nelem / pos > 2) {
                while (pos--) {
                        next = next->next;
                }
                prev = next->prev;
        } else {
                while (pos++ < this->nelem) {
                        prev = prev->prev;
                }
                next = prev->next;
        }
        list_node *new = calloc(1, sizeof(*new));
        new->value = calloc(1, this->size);
        memcpy(new->value, val, this->size);
        next->prev = new;
        prev->next = new;
        new->next = next;
        new->prev = prev;
        this->nelem++;
}

void *list_remove(list *this, u_int64_t pos);

void list_push_front(list *this, const void *val);
void *list_pop_front(list *this);
void list_push_back(list *this, const void *val);
void *list_pop_back(list *this);

void *list_set(list *this, u_int64_t pos, const void *val);
void *list_get(list *this, u_int64_t pos);
