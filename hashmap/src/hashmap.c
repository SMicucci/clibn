#include "../include/hashmap.h"
#include "../../include/vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct hashnode hashnode;
typedef struct hashbucket hashbucket;

struct hashmap {
        char *type;
        u_int64_t cap, size;
        hashbucket *bucket;
        vector *data;
};

struct hashnode {
        char *k;
        void *v;
};

struct hashbucket {
        u_int32_t size;
        vector *list;
};

/**
 * INTERNAL DECLARATION
 */

static inline u_int64_t hash(const char *key);
static inline hashbucket *get_bucket(hashmap *this, const char *key);
static inline int bucket_pos(vector *data, hashbucket *b, const char *key);

/**
 * EXTERNAL DEFINIITON
 */

hashmap *_hashmap_new(u_int64_t entries, u_int64_t size, const char *type)
{
        hashmap *this = calloc(1, sizeof(*this));
        // this is a risky move
        this->type = calloc(strlen(type) + 1, sizeof(*type));
        strcpy(this->type, type);
        this->cap = entries;
        this->size = size;
        // each bucket entry size is 0 by default
        this->bucket = calloc(this->cap, sizeof(hashbucket));
        this->data = vector_new(hashnode);
        return this;
}

void hashmap_delete(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        free(this->bucket);
        free(this->type);
        vector_delete(this->data);
        free(this);
}

const char *hashmap_type(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return this->type;
}
u_int64_t hashmap_nelem(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return vector_nelem(this->data);
}
u_int64_t hashmap_cap(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return this->cap;
}
u_int64_t hashmap_size(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return this->size;
}

void *hashmap_get(hashmap *this, const char *key)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        hashbucket *b = get_bucket(this, key);
        int pos = bucket_pos(this->data, b, key);
        if (pos == -1)
                return NULL;
        u_int32_t *data_pos = vector_get(b->list, pos);
        hashnode *node = vector_get(this->data, *data_pos);
        return node->v;
}

int hashmap_insert(hashmap *this, const char *key, const void *value)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        hashbucket *b = get_bucket(this, key);
        // check if exist already
        if (hashmap_exist(this, key)) {
                return -1;
        }
        // init bucket
        if (!b->size++) {
                b->list = vector_new(u_int64_t);
        }
        // create node
        hashnode *new = calloc(1, sizeof(*new));
        memcpy(new, value, this->size);
        // add entry into the bucket
        vector_push(b->list, (u_int32_t *)vector_nelem(this->data));
        // store the value
        vector_push(this->data, new);
        return 0;
}

void *hashmap_remove(hashmap *this, const char *key)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        if (!hashmap_exist(this, key))
                return NULL;
        hashbucket *b = get_bucket(this, key);
        u_int32_t b_pos = bucket_pos(this->data, b, key);
        u_int32_t d_pos = *(u_int32_t *)vector_get(b->list, b_pos);
        void *old_value = vector_remove(this->data, d_pos);
        return old_value;
}

void *hashmap_replace(hashmap *this, const char *key, const void *value)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        // check if not exist
        if (!hashmap_exist(this, key))
                return NULL;
        hashbucket *b = get_bucket(this, key);
        u_int32_t b_pos = bucket_pos(this->data, b, key);
        u_int32_t d_pos = *(u_int32_t *)vector_get(b->list, b_pos);
        void *old_value = vector_get(this->data, d_pos);
        vector_set(this->data, d_pos, value);
        return old_value;
}

int hashmap_exist(hashmap *this, const char *key)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        hashbucket *b = get_bucket(this, key);
        if (!b->size)
                return 0;
        // if not matched -1 + 1 => 0
        // else -nz
        return bucket_pos(this->data, b, key) + 1;
}

const char **hashmap_keys(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return NULL;
}

const void **hashmap_values(hashmap *this)
{
        assert(this && "not valid hashmap");
        assert(this->bucket && "not valid bucket");
        assert(this->data && "not valid data");
        return NULL;
}

/**
 * EXTERNAL DEFINIITON
 */

// dj2b hash
//
// "simple" hash function who is declared as well working
static inline u_int64_t hash(const char *key)
{
        u_int64_t hash = 5381;
        u_int8_t c;
        while ((c = *key++)) {
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash;
}

static inline hashbucket *get_bucket(hashmap *this, const char *key)
{
        u_int64_t entry = hash(key) % this->cap;
        hashbucket *b = this->bucket + (sizeof(*b) * entry);
        return b;
}

static inline int bucket_pos(vector *data, hashbucket *b, const char *key)
{
        // check bucket length
        if (b->size > 2 << 8) {
                perror("bucket over 256 entries, change hashmap size\n");
                return -1;
        }
        u_int32_t i = 0;
        while (i < b->size) {
                hashnode *node = vector_get(data, i);
                if (!strcmp(node->k, key))
                        return i;
                i++;
        }
        return -1;
}
