#include "../include/list.h"
#include <assert.h>
#include <stdio.h>
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
        strcpy(this->type, type);
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
        free(this);
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
        assert(pos <= this->nelem && "position out of list scope");
        if (!pos) {
                list_push_front(this, val);
                return;
        }
        list_node *next = this->head;
        list_node *prev = this->tail;
        // iterate through position
        u_int64_t cnt = pos;
        if (cnt / 2 < this->nelem) {
                while (cnt--) {
                        next = next->next;
                }
                prev = next->prev;
        } else {
                while (cnt++ < this->nelem) {
                        prev = prev->prev;
                }
                next = prev->next;
        }
        // generate and link new node
        list_node *new = calloc(1, sizeof(*new));
        new->value = calloc(1, this->size);
        memcpy(new->value, val, this->size);
        if (next)
                next->prev = new;
        if (prev)
                prev->next = new;
        new->next = next;
        new->prev = prev;
        // update list values
        if (pos == this->nelem) {
                this->tail = new;
        }
        if (!pos) {
                this->head = new;
        }
        this->nelem++;
}

void *list_remove(list *this, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert(pos <= this->nelem && "position out of list scope");
        if (!pos)
                return list_pop_front(this);
        // iterate through position
        list_node *src, *iter;
        u_int64_t cnt = pos;
        if (cnt / 2 < this->nelem) {
                iter = this->head;
                while (cnt--) {
                        iter = iter->next;
                }
                src = iter->prev;
        } else {
                iter = this->tail;
                while (cnt++ < this->nelem) {
                        iter = iter->prev;
                }
                src = iter->next;
        }
        // update link
        if (src->next)
                src->next->prev = src->prev;
        if (src->prev)
                src->prev->next = src->next;
        // update list
        if (pos == this->nelem) {
                this->tail = src->prev;
        }
        if (!pos) {
                this->head = src->next;
        }
        this->nelem--;
        // free resource and return value
        void *res = src->value;
        free(src);
        return res;
}

void list_push_front(list *this, const void *val)
{
        assert(this && "null list pointer");
        assert(val && "null value pointer");
        list_node *new = calloc(1, sizeof(*new));
        new->next = this->head;
        if (!this->nelem++) {
                this->tail = new;
        } else {
                this->head->prev = new;
        }
        this->head = new;
        new->value = calloc(1, this->size);
        memcpy(new->value, val, this->size);
}

void *list_pop_front(list *this)
{
        assert(this && "null list pointer");
        assert(this->nelem && "position out of list scope");
        list_node *trg = this->head;
        // set new head
        this->head = trg->next;
        // reduce size
        if (!--this->nelem) {
                this->tail = this->head = NULL;
        } else {
                this->head->prev = NULL;
        }
        void *res = trg->value;
        free(trg);
        return res;
}

void list_push_back(list *this, const void *val)
{
        assert(this && "null list pointer");
        assert(val && "null value pointer");
        list_node *new = calloc(1, sizeof(*new));
        new->prev = this->tail;
        if (!this->nelem++) {
                this->head = new;
        } else {
                this->tail->next = new;
        }
        this->tail = new;
        new->value = calloc(1, this->size);
        memcpy(new->value, val, this->size);
}

void *list_pop_back(list *this)
{
        assert(this && "null list pointer");
        assert(this->nelem && "position out of list scope");
        list_node *trg = this->tail;
        // set new head
        this->tail = trg->prev;
        // reduce size
        if (!--this->nelem) {
                this->head = this->tail = NULL;
        } else {
                this->tail->next = NULL;
        }
        void *res = trg->value;
        free(trg);
        return res;
}

void *list_set(list *this, u_int64_t pos, const void *val)
{
        assert(this && "null list pointer");
        assert(pos > this->nelem && "position out of list scope");
        // iterate through position
        list_node *src, *iter;
        if (this->nelem / pos > 2) {
                while (pos--) {
                        iter = iter->next;
                }
                src = iter->prev;
        } else {
                while (pos++ < this->nelem) {
                        iter = iter->prev;
                }
                src = iter->next;
        }
        // update link
        src->next->prev = src->prev;
        src->prev->next = src->next;
        // update list
        if (pos == this->nelem) {
                this->tail = src->prev;
        }
        if (!pos) {
                this->head = src->next;
        }
        this->nelem--;
        // return old value, assign new value
        void *res = src->value;
        src->value = calloc(1, this->size);
        memcpy(src->value, val, this->size);
        return res;
}

void *list_get(list *this, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert(pos > this->nelem && "position out of list scope");
        // iterate through position
        list_node *src, *iter;
        if (this->nelem / pos > 2) {
                while (pos--) {
                        iter = iter->next;
                }
                src = iter->prev;
        } else {
                while (pos++ < this->nelem) {
                        iter = iter->prev;
                }
                src = iter->next;
        }
        // update link
        src->next->prev = src->prev;
        src->prev->next = src->next;
        // update list
        if (pos == this->nelem) {
                this->tail = src->prev;
        }
        if (!pos) {
                this->head = src->next;
        }
        this->nelem--;
        // clone current value and return
        void *res = calloc(1, this->size);
        memcpy(res, src->value, this->size);
        return res;
}

//
// DEBUG PRINT
//

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

static inline void print_hex(void *arg, u_int64_t size)
{
        for (u_int64_t i = 0; i < size; i++) {
                printf("%02X", ((unsigned char *)arg)[i]);
                if (size - i - 1) {
                        printf("-");
                }
        }
        return;
}

void list_print(list *this, void (*print_value)(void *))
{
        assert(this);
        printf("%slist%s<%s%s%s>%s: %s%ld%s\n", C, Y, B, this->type, Y, X, S,
               this->nelem, D);
        printf("%s[head:%s%p%s | tail:%s%p%s]%s\n", S, G, this->head, X, G,
               this->tail, X, D);
        list_node *iter = this->head;
        while (iter) {
                printf("\t%s%p%s", R, iter, D);
                printf("%s[%s%s%p%s <- (%s", S, D, G, iter->prev, X, B);
                if (print_value) {
                        print_value(iter->value);
                } else {
                        print_hex(iter->value, this->size);
                }
                printf("%s) -> %s%p%s%s]%s\n", X, G, iter->next, X, S, D);
                iter = iter->next;
        }
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
