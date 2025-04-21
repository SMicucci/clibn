#pragma once

#include <sys/types.h>
typedef struct hashmap hashmap;

// use macro "hashmap_new(type)" (`hashmap *h + hashmap_new(2 << 10, long)`)
hashmap *_hashmap_new(u_int64_t entries, u_int64_t size, const char *type);
#ifndef hashmap_new
#define hashmap_new(entries, type) _hashmap_new(entries, sizeof(type), #type);
#endif
// free hasmap
void hashmap_delete(hashmap *this);

// return vector type label
const char *hashmap_type(hashmap *this);
// return current number of value stored
u_int64_t hashmap_nelem(hashmap *this);
// return static hash entry capacity
u_int64_t hashmap_cap(hashmap *this);
// return current element size
u_int64_t hashmap_size(hashmap *this);

// get value
void *hashmap_get(hashmap *this, const char *key);
// insert new value, return -1 if key already exist
int hashmap_insert(hashmap *this, const char *key, const void *value);
// remove key, return value pointer, if not exist return NULL
void *hashmap_remove(hashmap *this, const char *key);
// replace key, return old value pointer, if not exist return NULL
void *hashmap_replace(hashmap *this, const char *key, const void *value);

// return positive if exist, 0 if doesn't exist
int hashmap_exist(hashmap *this, const char *key);
// return a string array
//
// see hashmap_nelem for the size
const char **hashmap_keys(hashmap *this);
// return a void* array
//
// see hashmap_nelem for the size
const void **hashmap_values(hashmap *this);
