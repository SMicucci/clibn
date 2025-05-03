#include "../include/hashmap.h"
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
u_int64_t nohash(const char *key);
void print_int(void *v);
void print_float(void *v);

int main(void)
{
        // pseudo random keys for stress test
        const char *keys[] = {KEYS_LIST};
        u_int8_t limit = 5;
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
        /// hashmap_set
        ////////////////////////////////////////////////////////////////////////
        printf("> remove from hashmap\n");
        for (u_int8_t i = 0; i < limit; i++) {
                int *val = hashmap_remove(map, keys[i]);
                printf("\t\"%s\" -> %d\n", keys[i], *val);
                free(val);
        }
        hashmap_print(map, print_int);
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_delete
        ////////////////////////////////////////////////////////////////////////
        printf("> delete hashmap\n");
        hashmap_delete(map);
        map = NULL;
        ////////////////////////////////////////////////////////////////////////
        /// hashmap_set_hash
        ////////////////////////////////////////////////////////////////////////
        printf("> create new hashmap\n");
        map = hashmap_new(float);
        hashmap_print(map, print_float);
        printf("> add bad hash function to hashmap\n");
        hashmap_set_hash(map, nohash);
        return 0;
}

u_int64_t nohash(const char *key)
{
        FILE *fp = fopen("/dev/null", "w+");
        fprintf(fp, "%s\n", key);
        fclose(fp);
        return 0;
}

void print_int(void *v) { printf("%d", *(int *)v); }
void print_float(void *v) { printf("%f", *(float *)v); }
