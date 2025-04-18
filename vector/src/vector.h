#pragma once

#include <sys/types.h>

// simple vector implementation
typedef struct vector vector;

// use macro "vector_new(type)" (`vector *v = vector_new(u_int64_t);
vector *_vector_new(u_int64_t size, const char *type);
#ifndef vector_new
#define vector_new(type) _vector_new(sizeof(type), #type)
#endif

// free vector
void vector_delete(vector *this);
// return current size
u_int64_t vector_count(const vector *this);
// return current capacity
u_int64_t vector_cap(const vector *this);
// return current size
u_int64_t vector_size(const vector *this);
// return vector type label
const char *vector_type(const vector *this);

// set the value in any valid position
void vector_set(vector *this, u_int64_t pos, const void *val);
// get a copy of the value in valid any position
void *vector_get(const vector *this, u_int64_t pos);

// STACK IMPLEMENTATION

// append like a stack
void vector_push(vector *this, const void *val);
// extract like a stack (by value)
void *vector_pop(vector *this);

// ARRAY-LIST IMPLEMENTATION

// insert in any position and shift other
void vector_insert(vector *this, u_int64_t pos, const void *val);
// delete in any position and shift other
void *vector_remove(vector *this, u_int64_t pos);
