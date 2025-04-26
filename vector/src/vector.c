#include "../include/vector.h"
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef VECTOR_INIT_CAP
#define VECTOR_INIT_CAP 1 << 3
#endif

struct vector {
        char *type;
        u_int64_t size, cap, len;
        void *data;
};

/**
 * INTERNAL DECLARATION
 */

static inline void vector_grow(vector *this);
static inline void vector_shift_up(const vector *this, u_int64_t pos);
static inline void vector_shift_down(vector *this, u_int64_t pos);

/**
 * EXTERNAL IMPLEMENTATION
 */

vector *_vector_new(u_int64_t size, const char *type)
{
        vector *this = calloc(1, sizeof(*this));
        this->type = calloc(strlen(type) + 1, sizeof(char));
        strcpy(this->type, type);
        this->size = size;
        this->cap = VECTOR_INIT_CAP;
        this->len = 0;
        this->data = calloc(VECTOR_INIT_CAP, this->size);
        return this;
}

void vector_delete(vector *this)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid data");
        free(this->data);
        free((void *)this->type);
        free(this);
        return;
}

u_int64_t vector_len(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return this->len;
}

u_int64_t vector_cap(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return this->cap;
}

u_int64_t vector_size(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return this->size;
}

char *vector_type(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        char *res = calloc(strlen(this->type) + 1, 1);
        strcpy(res, this->type);
        return res;
}

void vector_set_at(vector *this, const void *val, u_int64_t pos)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        assert(val && "null val pointer");
        assert(pos < this->len && "access out of scope");
        void *trg = this->data + (this->size * pos);
        memcpy(trg, val, this->size);
}

void vector_set_first(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        // return vector_set_at(this, val, 0);
        void *trg = this->data + (this->size);
        memcpy(trg, val, this->size);
}

void vector_set_last(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        // return vector_set_at(this, val, this->len - 1);
        void *trg = this->data + (this->size * (this->len - 1));
        memcpy(trg, val, this->size);
}

void *vector_peek_at(const vector *this, u_int64_t pos)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        assert(pos < this->len && "access out of scope");
        void *val = calloc(1, this->size);
        memcpy(val, this->data + (this->size * pos), this->size);
        return val;
}

void *vector_peek_first(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        return vector_peek_at(this, 0);
}

void *vector_peek_last(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        return vector_peek_at(this, this->len - 1);
}

void vector_insert_at(vector *this, const void *val, u_int64_t pos)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        assert(val && "null val pointer");
        assert(pos <= this->len && "access out of scope");
        vector_grow(this);
        this->len++;
        vector_shift_up(this, pos);
        void *dest = this->data + this->size * pos;
        memcpy(dest, val, this->size);
}

void vector_insert_first(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        vector_insert_at(this, val, 0);
}

void vector_insert_last(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        vector_insert_at(this, val, this->len);
}

void *vector_remove_at(vector *this, u_int64_t pos)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        assert(this->len > pos && "position out of scope");
        this->len--;
        void *res = calloc(1, this->size);
        void *src = this->data + (this->len * this->size);
        memcpy(res, src, this->size);
        vector_shift_down(this, pos);
        return res;
}

void *vector_remove_first(vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        return vector_remove_at(this, 0);
}

void *vector_remove_last(vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        // naive implementation
        return vector_remove_at(this, this->len - 1);
}

/**
 * INTERNAL DEFINITION
 */

static inline void vector_grow(vector *this)
{
        // continue when is full
        if (this->len < this->cap)
                return;
        // double memory
        this->cap <<= 1;
        this->data = realloc(this->data, this->cap * this->size);
        assert(this->data && "failed realloc, run out of memory");
        // zeroing new memory
        void *new_mem = this->data + (this->size * (this->cap >> 1));
        memset(new_mem, 0, this->size * (this->cap >> 1));
        return;
}

// this->len = 8 (pre-incremented from 7)
// pos = 4
//                 (moved)      (len)
//                    |           |- (prev)
//                    v           v
//  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
//    |   |   |   |    \   \   \      x
//    |   |   |   |     \   \   \     |
//    |   |   |   |      \   \   \    x
//  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
//    i:              ^  [2] [1] [0]
//                    |
//                  (len)

static inline void vector_shift_up(const vector *this, u_int64_t pos)
{
        assert(pos < this->len && "max capacity, forward shift impossible");
        u_int64_t i = this->len - 1;
        while (i > pos) {
                void *s = this->data + (this->size * (i - 1));
                void *d = this->data + (this->size * i);
                memcpy(d, s, this->size);
                i--;
        }
        void *clean = this->data + (this->size * pos);
        memset(clean, 0, this->size);
}

// this->len = 7 (pre-decremented from 8)
// pos = 4
//                (removed)         (len)
//                    |       (prev) -|
//                    v               v
//  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
//    |   |   |   |      /   /   /    x
//    |   |   |   |     /   /   /
//    |   |   |   |    /   /   /  x   x
//  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
//    i:             [0] [1] [2]  ^
//                                |
//                              (len)

static inline void vector_shift_down(vector *this, u_int64_t pos)
{
        if (!this->len)
                return;
        u_int64_t i = pos;
        while (i < this->len - 1) {
                void *s = this->data + (this->size * (i + 1));
                void *d = this->data + (this->size * i);
                memcpy(d, s, this->size);
                i++;
        }
        void *clean = this->data + (this->size * (this->len));
        memset(clean, 0, this->size);
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
                unsigned char *p = arg;
                printf("%02X", p[i]);
                if (size - i - 1) {
                        printf("-");
                }
        }
}

void vector_print(vector *this, void (*print_value)(void *))
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        printf("%svector%s<%s%s%s>%s", C, Y, B, this->type, Y, D);
        printf(": %s%lu%s [%s%lu%s]\n", S, this->len, D, R, this->cap, D);
        for (u_int64_t i = 0; i < this->cap; i++) {
                void *val = this->data + (this->size * i);
                if (print_value && this->len > i) {
                        printf("%s%2lu:%s [%s", G, i, D, B);
                        print_value(val);
                } else {
                        printf("%s%2lu:%s [%s", R, i, D, P);
                        print_hex(val, this->size);
                }
                printf("%s]", D);
                if ((i + 1) % 4) {
                        printf("  ");
                } else {
                        printf("\n");
                }
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
