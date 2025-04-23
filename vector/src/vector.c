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
        memcpy(this->data + (this->size * pos), val, this->size);
}

void vector_set_first(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return vector_set_at(this, val, 0);
}

void vector_set_last(vector *this, const void *val)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return vector_set_at(this, val, this->len - 1);
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
        return vector_peek_at(this, 0);
}

void *vector_peek_last(const vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
        return vector_peek_at(this, this->len - 1);
}

// void *vector_remove_last(vector *this)
// {
//         assert(this && "null vector pointer");
//         assert(this->data && "not valid vector");
//         if (!this->len) {
//                 return NULL;
//         }
//         // nelem point to first good entry
//         void *src = this->data + (this->size * --this->len);
//         void *res = calloc(1, this->size);
//         // number of element point to the next entry
//         memcpy(res, src, this->size);
//         memset(src, 0, this->size);
//         return res;
// }

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
        // if (!(this->len - pos - 1)) {
        //         return vector_remove_last(this);
        // }
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
        return vector_remove_at(this, 0);
}

void *vector_remove_last(vector *this)
{
        assert(this && "null vector pointer");
        assert(this->data && "not valid vector");
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
        this->cap <<= 1;
        this->data = realloc(this->data, this->cap * this->size);
        assert(this->data && "failed realloc, run out of memory");
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
