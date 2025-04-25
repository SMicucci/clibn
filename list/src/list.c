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
        u_int64_t len, size;
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
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
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

u_int64_t list_len(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        return this->len;
}

u_int64_t list_size(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        return this->size;
}

const char *list_type(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        char *res = calloc(1, strlen(this->type) + 1);
        strcpy(res, this->type);
        return res;
}

void list_insert_at(list *this, const void *val, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(val && "null value pointer");
        assert(this->len + 1 > pos && "position out of scope");
        list_node *n = NULL, *p = NULL;
        u_int64_t c;
        if (pos < this->len / 2) {
                n = this->head;
                c = pos;
                while (c--) {
                        p = n;
                        n = n->next;
                }
                if (n)
                        p = n->prev;
        } else {
                p = this->tail;
                c = this->len - pos;
                while (c--) {
                        n = p;
                        p = p->prev;
                }
                if (p)
                        n = p->next;
        }
        list_node *d = calloc(1, sizeof(*d));
        // linking and list entries
        d->next = n;
        if (n) {
                n->prev = d;
        } else {
                this->tail = d;
        }
        d->prev = p;
        if (p) {
                p->next = d;
        } else {
                this->head = d;
        }
        // storing values
        d->value = calloc(1, this->size);
        memcpy(d->value, val, this->size);
        this->len++;
        return;
}

void list_insert_first(list *this, const void *val)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(val && "null value pointer");
        list_insert_at(this, val, 0);
}

void list_insert_last(list *this, const void *val)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(val && "null value pointer");
        list_insert_at(this, val, this->len);
}

void *list_remove_at(list *this, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(this->len > pos && "position out of list scope");
        list_node *iter;
        u_int64_t cnt;
        this->len--;
        if (pos < this->len / 2) {
                iter = this->head;
                cnt = pos;
                while (cnt--) {
                        iter = iter->next;
                }
        } else {
                iter = this->tail;
                cnt = this->len - pos;
                while (cnt--) {
                        iter = iter->prev;
                }
        }
        void *res = iter->value;
        // check list entry
        if (iter == this->head)
                this->head = iter->next;
        if (iter == this->tail)
                this->tail = iter->prev;
        // check linkage
        if (iter->next) {
                iter->next->prev = iter->prev;
        }
        if (iter->prev) {
                iter->prev->next = iter->next;
        }
        free(iter);
        return res;
}

void *list_remove_first(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(this->len && "position out of list scope");
        list_node *trg = this->head;
        // set new head
        this->head = trg->next;
        // reduce size
        if (!--this->len) {
                this->tail = this->head = NULL;
        } else {
                this->head->prev = NULL;
        }
        void *res = trg->value;
        free(trg);
        return res;
}

void *list_remove_last(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(this->len && "position out of list scope");
        list_node *trg = this->tail;
        // set new head
        this->tail = trg->prev;
        // reduce size
        if (!--this->len) {
                this->head = this->tail = NULL;
        } else {
                this->tail->next = NULL;
        }
        void *res = trg->value;
        free(trg);
        return res;
}

void list_set_at(list *this, const void *val, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(this->len > pos && "position out of list scope");
        // iterate through position
        list_node *trg;
        if (this->len / 2 > pos) {
                trg = this->head;
                while (pos--) {
                        trg = trg->next;
                }
        } else {
                trg = this->tail;
                while (pos++ < this->len) {
                        trg = trg->prev;
                }
        }
        // assign new value
        memcpy(trg->value, val, this->size);
        return;
}

void list_set_first(list *this, const void *val) { list_set_at(this, val, 0); }

void list_set_last(list *this, const void *val)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        list_set_at(this, val, this->len - 1);
}

void *list_peek_at(list *this, u_int64_t pos)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        assert(this->len > pos && "position out of list scope");
        void *res = calloc(1, this->size);
        // iterate through position
        list_node *trg;
        if (this->len / 2 > pos) {
                trg = this->head;
                while (pos--) {
                        trg = trg->next;
                }
        } else {
                trg = this->tail;
                while (pos++ < this->len) {
                        trg = trg->prev;
                }
        }
        // return value
        memcpy(res, trg->value, this->size);
        return res;
}

void *list_peek_first(list *this) { return list_peek_at(this, 0); }

void *list_peek_last(list *this)
{
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        return list_peek_at(this, this->len - 1);
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
        assert(this && "null list pointer");
        assert((!this->len || this->head || this->tail) && "not valid list");
        printf("%slist%s<%s%s%s>%s: %s%ld%s\n", C, Y, B, this->type, Y, X, S,
               this->len, D);
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
