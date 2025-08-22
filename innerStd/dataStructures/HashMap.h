#ifndef HASH_MAP
#define HASH_MAP

#include "LinkedList.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct HashMapNode {
    bool(*are_keys_equal)(const void*, const void*);
    void* key;
    void* val;
} HashMapNode;

typedef struct HashMap {
    LinkedList* lists_with_nodes;
    size_t(*hash_func)(const void*);
    bool(*are_keys_equal)(const void*, const void*);
    size_t array_capacity;
    size_t size;
} HashMap;

void hash_map_init(HashMap* map, size_t(*hash_func)(const void*), bool(*are_keys_equal)(const void*, const void*));
void hash_map_add(HashMap* map, void* key, void* val);
void* hash_map_get(const HashMap* map, const void* key);
void hash_map_clear(HashMap* map);

#endif