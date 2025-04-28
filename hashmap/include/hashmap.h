#pragma once

#include <sys/types.h>

typedef struct hashmap hashmap;

hashmap *_hashmap_new(u_int64_t size, const char *type);
#ifndef hashmap_new
#define hashmap_new(type) _hashmap_new(sizeof(type), #type);
#endif
void hashmap_delete(hashmap *this);

const char *hashmap_type(hashmap *this);
u_int64_t hashmap_len(hashmap *this);
u_int64_t hashmap_cap(hashmap *this);
u_int64_t hashmap_size(hashmap *this);

void hashmap_set(hashmap *this, const char *key, const void *value);
void *hashmap_peek(hashmap *this, const char *key);
void hashmap_insert(hashmap *this, const char *key, const void *value);
void *hashmap_remove(hashmap *this, const char *key);

/// SPECIFIC HASHMAP FUNCTION

void hashmap_set_hash(hashmap *this, u_int64_t (*hash)(const char *key));
// void hashmap_reset_hash(hashmap *this);
const char **hashmap_keys(hashmap *this);
const void **hashmap_values(hashmap *this);
