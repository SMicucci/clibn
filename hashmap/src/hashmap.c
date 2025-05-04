#include "../include/hashmap.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef NULL
#define NULL 0
#endif

#define RH_PROBE_EMPTY 0x80
#define RH_PROBE_TOMB 0x40
#define RH_PROBE_JUMP_MASK 0x3F

#ifndef HASHMAP_INIT_CAP
#define HASHMAP_INIT_CAP (1 << 2)
#endif
#ifndef HASHMAP_THRESHOLD
#define HASHMAP_THRESHOLD 0.85
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

u_int64_t default_hash(const char *key);
static inline void hashmap_grow(hashmap *this);
static inline hash_node *hashmap_find(hashmap *this, const char *key, int sk_t);
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
        this->hash = default_hash;
        this->type = calloc(strlen(type) + 1, 1);
        strcpy(this->type, type);
        this->data = calloc(this->cap, sizeof(hash_node));
        for (u_int64_t i = 0; i < this->cap; i++) {
                this->data[i] = (hash_node){RH_PROBE_EMPTY, NULL, NULL};
        }
        return this;
}

void hashmap_delete(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
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

char *hashmap_type(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        char *res = calloc(strlen(this->type) + 1, 1);
        strcpy(res, this->type);
        return res;
}

u_int64_t hashmap_len(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        return this->len;
}

u_int64_t hashmap_cap(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        return this->cap;
}

u_int64_t hashmap_size(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        return this->size;
}

void hashmap_set(hashmap *this, const char *key, const void *value)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        hash_node *trg = hashmap_find(this, key, 1);
        memcpy(trg->value, value, this->size);
        return;
}

void *hashmap_peek(hashmap *this, const char *key)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        hash_node *trg = hashmap_find(this, key, 1);
        void *res = calloc(1, this->size);
        memcpy(res, trg->value, this->size);
        return res;
}

void hashmap_insert(hashmap *this, const char *key, const void *value)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        this->len++;
        hashmap_grow(this);
        hash_node *trg = hashmap_find(this, key, 0);
        trg->key = calloc(strlen(key) + 1, 1);
        trg->value = calloc(1, this->size);
        strcpy(trg->key, key);
        memcpy(trg->value, value, this->size);
        return;
}

void *hashmap_remove(hashmap *this, const char *key)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        hash_node *trg = hashmap_find(this, key, 1);
        this->len--;
        void *res = trg->value;
        free(trg->key);
        trg->value = NULL;
        trg->key = NULL;
        trg->rh_probe = RH_PROBE_TOMB;
        return res;
}

void hashmap_set_hash(hashmap *this, u_int64_t (*hash)(const char *key))
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        if (!hash && this->hash == default_hash)
                return;
        if (hash == this->hash)
                return;
        if (!hash) {
                this->hash = default_hash;
        } else {
                this->hash = hash;
        }
        hashmap_rehash(this);
        return;
}

char **hashmap_keys(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        char **res = calloc(this->len, sizeof(*res));
        u_int64_t pos = 0;
        for (u_int64_t i = 0; i < this->cap; i++) {
                assert(pos <= this->len && "write out of scope");
                hash_node node = this->data[i];
                if (node.rh_probe & (RH_PROBE_EMPTY | RH_PROBE_TOMB))
                        continue;
                res[pos] = calloc(strlen(node.key) + 1, 1);
                strcpy(res[pos++], node.key);
        }
        return res;
}

void **hashmap_values(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        void **res = calloc(this->len, sizeof(*res));
        u_int64_t pos = 0;
        for (u_int64_t i = 0; i < this->cap; i++) {
                assert(pos <= this->len && "write out of scope");
                hash_node node = this->data[i];
                if (node.rh_probe & (RH_PROBE_EMPTY | RH_PROBE_TOMB))
                        continue;
                res[pos] = calloc(1, this->size);
                memcpy(res[pos++], node.value, this->size);
        }
        return res;
}

/**
 * INTERNAL IMPLEMENTATION
 */

u_int64_t default_hash(const char *key)
{
        u_int64_t hash = 5381;
        u_int64_t c;
        while ((c = (u_int64_t)*key++)) {
                hash = ((hash << 5) + hash) + c;
        }
        return hash;
}

static inline void hashmap_grow(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        float fullness = (float)(this->len) / (float)(this->cap);
        if (HASHMAP_THRESHOLD > fullness)
                return;
        hash_node *old = this->data;
        this->cap <<= 1;
        this->data = calloc(this->cap, sizeof(*this->data));
        // init new data
        for (u_int64_t i = 0; i < this->cap; i++) {
                this->data[i] = (hash_node){RH_PROBE_EMPTY, NULL, NULL};
        }
        // re-insert nodes
        if (!this->len)
                return;
        for (u_int64_t i = 0; i < this->cap >> 1; i++) {
                hash_node *iter = &old[i];
                if (iter->rh_probe & (RH_PROBE_EMPTY | RH_PROBE_TOMB)) {
                        continue;
                }
                hash_node *trg = hashmap_find(this, iter->key, 0);
                trg->key = iter->key;
                trg->value = iter->value;
        }
        free(old);
}

static inline hash_node *hashmap_find(hashmap *this, const char *key, int sk_t)
{
        assert(this && "null hashmap pointer");
        assert(key && "null key pointer");
        u_int64_t pos = this->hash(key) % this->cap;
        u_int8_t jump = 0;
        while (1) {
                // traingulation {(j^2 + j)/2}
                u_int64_t offset = ((jump * jump + jump) >> 1);
                u_int64_t index = (pos + offset) % this->cap;
                hash_node *iter = &(this->data[index]);
                // available slot
                if (iter->rh_probe & RH_PROBE_EMPTY) {
                        iter->rh_probe = (jump & RH_PROBE_JUMP_MASK);
                        return iter;
                }
                if (iter->key && !strcmp(iter->key, key))
                        return iter;
                // insert case
                if (!sk_t) {
                        if (iter->rh_probe & RH_PROBE_TOMB) {
                                iter->rh_probe = (jump & RH_PROBE_JUMP_MASK);
                                return iter;
                        }
                        // robin hood swap
                        u_int8_t i_jump = iter->rh_probe & RH_PROBE_JUMP_MASK;
                        if (i_jump < jump && iter->key) {
                                char *k = iter->key;
                                void *v = iter->value;
                                iter->rh_probe = (jump & RH_PROBE_JUMP_MASK);
                                iter->key = NULL;
                                iter->value = NULL;
                                hash_node *t = hashmap_find(this, k, sk_t);
                                t->key = k;
                                t->value = v;
                                return iter;
                        }
                }
                jump++;
                assert(jump < RH_PROBE_JUMP_MASK &&
                       "too many collision, weak hash or too full");
        }
}

static inline void hashmap_rehash(hashmap *this)
{
        assert(this && "null hashmap pointer");
        assert(this->data && "invalid hashmap pointer");
        if (!this->len)
                return;
        hash_node *old = this->data;
        this->data = calloc(this->cap, sizeof(*this->data));
        // init new data
        for (u_int64_t i = 0; i < this->cap; i++) {
                this->data[i] = (hash_node){RH_PROBE_EMPTY, NULL, NULL};
        }
        // insertion
        for (u_int64_t i = 0; i < this->cap; i++) {
                hash_node *iter = &old[i];
                if (iter->rh_probe & (RH_PROBE_EMPTY | RH_PROBE_TOMB)) {
                        continue;
                }
                hash_node *trg = hashmap_find(this, iter->key, 0);
                trg->key = iter->key;
                trg->value = iter->value;
        }
        free(old);
        return;
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

void hashmap_print(hashmap *this, void (*print_value)(void *))
{
        assert(this && "null hashmap pointer");
        assert(this->data && "not valid hashmap");
        printf("%shashmap%s<%s%s%s>%s: ", C, Y, B, this->type, Y, D);
        printf("%s%lu%s [%s%lu%s]\n", S, this->len, s, R, this->cap, D);
        u_int64_t i = 0;
        while (i < this->cap) {
                hash_node *trg = this->data + i++;
                u_int8_t empty_trg =
                    trg->rh_probe & (RH_PROBE_EMPTY | RH_PROBE_TOMB);
                if (empty_trg) {
                        if (trg->rh_probe & RH_PROBE_TOMB) {
                                printf("\t<%s0x%02x%s> ", Y, trg->rh_probe, D);
                        } else {
                                printf("\t<%s0x%02x%s> ", R, trg->rh_probe, D);
                        }
                        printf("%s%p%s:", P, trg->key, D);
                        printf(" %s%p%s\n", P, trg->value, D);
                } else {
                        printf("\t<%s0x%02x%s> ", G, trg->rh_probe, D);
                        printf("\"%s%s%s\":", B, trg->key, D);
                        printf(" [%s", B);
                        if (print_value) {
                                print_value(trg->value);
                        } else {
                                print_hex(trg->value, this->size);
                        }
                        printf("%s]\n", D);
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
