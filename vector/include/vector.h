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

char *vector_type(const vector *this);
u_int64_t vector_len(const vector *this);
u_int64_t vector_cap(const vector *this);
u_int64_t vector_size(const vector *this);

void vector_set_at(vector *this, const void *val, u_int64_t pos);
void vector_set_first(vector *this, const void *val);
void vector_set_last(vector *this, const void *val);

void *vector_peek_at(const vector *this, u_int64_t pos);
void *vector_peek_first(const vector *this);
void *vector_peek_last(const vector *this);

void vector_insert_at(vector *this, const void *val, u_int64_t pos);
void vector_insert_first(vector *this, const void *val);
void vector_insert_last(vector *this, const void *val);

void *vector_remove_at(vector *this, u_int64_t pos);
void *vector_remove_first(vector *this);
void *vector_remove_last(vector *this);
