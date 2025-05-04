#include "../include/hashmap.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define KEYS_LIST                                                              \
        "casa", "auto", "computer portatile", "telefono cellulare",            \
            "lampada da tavolo", "tavolo in legno", "sedia pieghevole",        \
            "bicicletta da corsa", "motore diesel", "motore benzina",          \
            "motore elettrico", "caffetteria", "pane integrale",               \
            "zona industriale", "giardino pubblico", "libro di testo",         \
            "film documentario", "scarpe da ginnastica", "borsa di pelle",     \
            "cancello automatico", "aaaa", "aaab", "aaba", "abaa", "baaa",     \
            "aaaaa", "aaaab", "aaaba", "aabaa", "abaaa", "baaaa", "abc",       \
            "acb", "bac", "bca", "cab", "cba", "az", "zzzz", "zzzy", "zzyz",   \
            "zyzz", "yzzz", "za", "aaaaz", "aaaza", "xxxx", "xxxxy", "xxxyx",  \
            "xxyxx", "xyxxx", "yxxxx", "yxxxy", "xyxxy", "xxyxy", "xxyyy",     \
            "xyxyy", "invoice", "client", "server", "cache", "session",        \
            "request", "database", "query", "transaction", "payload",          \
            "endpoint", "token", "api", "billing", "timeout", "user",          \
            "encryption", "protocol", "gateway", "backend", "aazaa", "azaaa",  \
            "zaaaa", "stylesheet", "html", "javascript", "cookieJar",          \
            "responsive", "ajax", "cors", "websocket", "dom", "viewport",      \
            "favicon", "router", "middleware", "csrf", "json", "bootstrap",    \
            "svg", "webhook", "pagination", "crossorigin"

extern void hashmap_print(hashmap *this, void (*print_value)(void *));
u_int64_t weakhash(const char *key);
void print_int(void *v);
// void print_float(void *v);

int main(void)
{
        // pseudo random keys for stress test
        const char *keys[] = {KEYS_LIST};
        u_int8_t limit = 20;
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_new
        ////////////////////////////////////////////////////////////////////////
        printf("> new hashmap\n");
        hashmap *map = hashmap_new(int);
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_insert
        ////////////////////////////////////////////////////////////////////////
        printf("> insert into hashmap\n");
        for (u_int8_t i = 0; i < limit; i++) {
                int val = ((i + 1) << 4) + (i + 1);
                hashmap_insert(map, keys[i], &val);
                printf("\t\"%s\" -> %d\n", keys[i], val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_peek
        ////////////////////////////////////////////////////////////////////////
        printf("> peek from hashmap\n");
        for (u_int8_t i = 0; i < limit; i++) {
                int *val = hashmap_peek(map, keys[i]);
                printf("\t\"%s\" -> %d\n", keys[i], *val);
                free(val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_set
        ////////////////////////////////////////////////////////////////////////
        printf("> set into hashmap\n");
        for (u_int8_t i = 0; i < limit; i++) {
                int val = i + 1;
                hashmap_set(map, keys[i], &val);
                printf("\t\"%s\" -> %d\n", keys[i], val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_remove
        ////////////////////////////////////////////////////////////////////////
        printf("> remove half from hashmap\n");
        for (u_int8_t i = 0; i < (limit / 2); i++) {
                int *val = hashmap_remove(map, keys[i]);
                printf("\t\"%s\" -> %d\n", keys[i], *val);
                free(val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_insert
        ////////////////////////////////////////////////////////////////////////
        printf("> insert new into hashmap\n");
        for (u_int8_t i = limit; i < (limit << 1); i++) {
                int val = ((i + 1) << 4) + (i + 1);
                hashmap_insert(map, keys[i], &val);
                printf("\t\"%s\" -> %d\n", keys[i], val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_set_hash
        ////////////////////////////////////////////////////////////////////////
        printf("> add bad hash function to hashmap\n");
        hashmap_set_hash(map, weakhash);
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_set_hash
        ////////////////////////////////////////////////////////////////////////
        printf("> reset hash function\n");
        hashmap_set_hash(map, NULL);
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_keys
        ////////////////////////////////////////////////////////////////////////
        printf("> get hashmap keys\n");
        char **arr_keys = hashmap_keys(map);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_values
        ////////////////////////////////////////////////////////////////////////
        printf("> get hashmap keys\n");
        int **arr_vals = (int **)hashmap_values(map);
        hashmap_print(map, print_int);
        printf("> print pairs key-value\n");
        for (u_int64_t i = 0; i < hashmap_len(map); i++) {
                // printf("%lu -> \"%s\"\n", i, arr_keys[i]);
                printf("%lu -> \"%s\" [%d]\n", i, arr_keys[i], *arr_vals[i]);
                free(arr_keys[i]);
                free(arr_vals[i]);
        }
        free(arr_keys);
        arr_keys = NULL;
        free(arr_vals);
        arr_vals = NULL;
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_delete
        ////////////////////////////////////////////////////////////////////////
        printf("> delete hashmap\n");
        hashmap_delete(map);
        map = NULL;
        return 0;
}

u_int64_t weakhash(const char *key)
{
        u_int64_t hash = 0;
        for (u_int64_t i = 0; i < strlen(key); i++) {
                hash += (u_int64_t)key[i];
        }
        return hash;
}

void print_int(void *v) { printf("%d", *(int *)v); }
// void print_float(void *v) { printf("%f", *(float *)v); }
