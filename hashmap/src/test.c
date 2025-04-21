#include "../include/hashmap.h"

int main(void)
{
        hashmap *map = hashmap_new(2 << 10, int);
        hashmap_delete(map);
        return 0;
}
