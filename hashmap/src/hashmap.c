#include "../include/hashmap.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef RH_PROBE_EMPTY
#define RH_PROBE_EMPTY 0x80
#endif
#ifndef RH_PROBE_TOMB
#define RH_PROBE_TOMB 0x40
#endif
#ifndef RH_PROBE_JUMP_MASK
#define RH_PROBE_JUMP_MASK 0x3F
#endif

#ifndef HASHMAP_INIT_CAP
#define HASHMAP_INIT_CAP 1 << 5
#endif

typedef struct {
        u_int8_t rh_probe;
        char *key;
        void *value;
} hash_node;

struct hashmap {
        char *type;
        u_int64_t cap, size, len;
        hash_node *data;
        u_int64_t (*hash)(const char *key);
};

/**
 * INTERNAL IMPLEMENTATION
 */

static inline void hashmap_grow(hashmap *this);
static inline void hashmap_rehash(hashmap *this);

/**
 * EXTERNAL IMPLEMENTATION
 */

hashmap *_hashmap_new(u_int64_t size, const char *type)
{
        hashmap *this = calloc(1, sizeof(*this));
        this->cap = HASHMAP_INIT_CAP;
        this->len = 0;
        this->size = size;
        this->hash = NULL;
        this->type = calloc(strlen(type) + 1, 1);
        strcpy(this->type, type);
        this->data = calloc(this->cap, sizeof(hash_node));
        for (u_int64_t i = 0; i < this->cap; i++) {
                hash_node trg = this->data[i];
                trg.rh_probe = RH_PROBE_EMPTY;
                trg.key = NULL;
                trg.value = NULL;
        }
        return this;
}

void hashmap_delete(hashmap *this)
{
        assert(this && "null hashmap pointer");
        for (u_int64_t i = 0; i < this->cap; i++) {
                hash_node trg = this->data[i];
                if (trg.rh_probe & RH_PROBE_EMPTY ||
                    trg.rh_probe & RH_PROBE_TOMB)
                        continue;
                free(trg.key);
                free(trg.value);
        }
        free(this->data);
        free(this->type);
        free(this);
}

const char *hashmap_type(hashmap *this);
u_int64_t hashmap_len(hashmap *this);
u_int64_t hashmap_cap(hashmap *this);
u_int64_t hashmap_size(hashmap *this);

void hashmap_set(hashmap *this, const char *key, const void *value);
void *hashmap_peek(hashmap *this, const char *key);
void hashmap_insert(hashmap *this, const char *key, const void *value);
void *hashmap_remove(hashmap *this, const char *key);

void hashmap_set_hash(hashmap *this, u_int64_t (*hash)(const char *key));
void hashmap_reset_hash(hashmap *this);
const char **hashmap_keys(hashmap *this);
const void **hashmap_values(hashmap *this);

/**
 * INTERNAL IMPLEMENTATION
 */

static inline void _hashmap_grow(hashmap *this);

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

void hashmap_print(hashmap *this, void (*print_value)(void *));

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
