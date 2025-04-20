#include "../include/vector.h"
#include <asm-generic/errno-base.h>
#include <assert.h>
#include <errno.h>
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
        const char *type;
        u_int64_t size, cap, nelem;
        void *data;
};

/**
 * INTERNAL DECLARATION
 */
typedef enum _direction { FORWARD, BACKWARD } direction;
static inline int _vector_grow(vector *this);
static inline int _vector_shift(vector *this, u_int64_t from, direction dir);

/**
 * EXTERNAL IMPLEMENTATION
 */
vector *_vector_new(u_int64_t size, const char *type)
{
        vector *this = calloc(1, sizeof(*this));
        char *_type = calloc(strlen(type) + 1, sizeof(char));
        strcpy(_type, type);
        this->type = (const char *)_type;
        this->size = size;
        this->cap = VECTOR_INIT_CAP;
        this->nelem = 0;
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

u_int64_t vector_nelem(const vector *this) { return this->nelem; }
u_int64_t vector_cap(const vector *this) { return this->cap; }
u_int64_t vector_size(const vector *this) { return this->size; }
const char *vector_type(const vector *this) { return this->type; }

void vector_set(vector *this, u_int64_t pos, const void *val)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(val && "not valid value");
        if (this->nelem < pos) {
                errno = EINVAL;
        }
        memcpy(this->data + (this->size * pos), val, this->size);
}

void *vector_get(const vector *this, u_int64_t pos)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(this->nelem > pos && "position out of scope");
        void *val = calloc(1, this->size);
        memcpy(val, this->data + (this->size * pos), this->size);
        return val;
}

void vector_push(vector *this, const void *val)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(val && "not valid value");
        if (_vector_grow(this)) {
                perror(strerror(errno));
                return;
        }
        memcpy(this->data + (this->nelem++ * this->size), val, this->size);
        return;
}

void *vector_pop(vector *this)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        if (!this->nelem) {
                return NULL;
        }
        // nelem point to first good entry
        void *src = this->data + (this->size * --this->nelem);
        void *res = calloc(1, this->size);
        // number of element point to the next entry
        memcpy(res, src, this->size);
        memset(src, 0, this->size);
        return res;
}

void vector_insert(vector *this, u_int64_t pos, const void *val)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(val && "not valid value");
        if (!(this->nelem - pos)) {
                vector_push(this, val);
                return;
        }
        if (_vector_grow(this)) {
                perror(strerror(errno));
                return;
        }
        this->nelem++;
        _vector_shift(this, pos, FORWARD);
        memcpy(this->data + this->size * pos, val, this->size);
}

void *vector_remove(vector *this, u_int64_t pos)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(this->nelem > pos && "position out of scope");
        if (!(this->nelem - pos - 1)) {
                return vector_pop(this);
        }
        void *res = calloc(1, this->size);
        void *src = this->data + (--this->nelem * this->size);
        memcpy(res, src, this->size);
        _vector_shift(this, pos, BACKWARD);
        return res;
}

/**
 * INTERNAL DEFINITION
 */

// internal check for growing memory
//
// don't increment counter
static inline int _vector_grow(vector *this)
{
        // continue when is full
        if (this->cap - this->nelem)
                return 0;
        this->cap <<= 1;
        void *__old = this->data;
        this->data = realloc(__old, this->cap * this->size);
        // realloc failed (NULL return)
        assert(this->data && "reallocation failed, run out of memory");
        return 0;
}

// shift every last `(n - from)` value according to dir.
// FORWARD: `from -> (from + 1)` ..  `[(n - 1) -> n]`
//
// BACKWARD: `(from + 1) -> from` .. `[(n + 1) -> n]`
static inline int _vector_shift(vector *this, u_int64_t from, direction dir)
{
        assert(this && "not valid vector");
        assert(this->data && "not valid vector (data)");
        assert(this->nelem > from && "position out of scope");
        if (!this || this->nelem < from) {
                errno = EINVAL;
                return -1;
        }
        switch (dir) {
                case FORWARD:
                        for (u_int64_t i = this->nelem; i > from; i--) {
                                void *dest = this->data + (this->size * i);
                                void *src = this->data + (this->size * (i - 1));
                                memcpy(dest, src, this->size);
                        }
                        // zeros double data
                        memset(this->data + this->size * from, 0, this->size);
                        return 0;
                case BACKWARD:
                        for (u_int64_t i = from; i < this->nelem; i++) {
                                void *dest = this->data + (this->size * i);
                                void *src = this->data + (this->size * (i + 1));
                                memcpy(dest, src, this->size);
                        }
                        // zeros double data
                        memset(this->data + this->size * (this->nelem + 1), 0,
                               this->size);
                        return 0;
                default:
                        assert(0 && "not valid direction");
        }
}
