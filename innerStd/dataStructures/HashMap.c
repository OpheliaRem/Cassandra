#include "HashMap.h"
#include "../allocator.h"

void hash_map_init(HashMap* map, size_t(*hash_func)(void*), bool(*are_keys_equal)(void*, void*)) {
    if (!map || !hash_func || !are_keys_equal) {
        return;
    }

    map->array_capacity = 512;
    map->size = 0;
    map->hash_func = hash_func;
    map->are_keys_equal = are_keys_equal;
    map->lists_with_nodes = (LinkedList*)allocate(map->array_capacity * sizeof(LinkedList));

    if (!(map->lists_with_nodes)) {
        hash_map_clear(map);
        return;
    }

    for (size_t i = 0; i < map->array_capacity; ++i) {
        linked_list_init(map->lists_with_nodes + i);
    }
}

static bool find_existing_key(void* v_node, void* key) {
    HashMapNode* node = (HashMapNode*)v_node;
    return node->are_keys_equal(node->key, key);
}

static void resize(HashMap* map, size_t capacity) {
    LinkedList* new_lists = (LinkedList*)allocate(capacity * sizeof(LinkedList));

    if (!new_lists) {
        return;
    }

    map->array_capacity = capacity;

    for (size_t i = 0; i < map->size; ++i) {
        new_lists[i] = map->lists_with_nodes[i];
    }

    free(map->lists_with_nodes);
    map->lists_with_nodes = new_lists;
}

void hash_map_add(HashMap* map, void* key, void* val) {
    HashMapNode* new_node = (HashMapNode*)allocate(sizeof(HashMapNode));
    
    if (!new_node) {
        return;
    }
    
    new_node->are_keys_equal = map->are_keys_equal;
    new_node->key = key;
    new_node->val = val;
    
    const size_t index = map->hash_func(key) % map->array_capacity;

    LinkedListNode* node_with_given_key = linked_list_search(&map->lists_with_nodes[index], key, find_existing_key);
    
    if (node_with_given_key) {
        free(node_with_given_key->data);
        node_with_given_key->data = new_node;
    } else {
        linked_list_push_back(&map->lists_with_nodes[index], new_node);
    }

    map->size++;

    if (map->size > 0.75 * map->array_capacity) {
        resize(map, map->array_capacity * 2);
    }
}

void* hash_map_get(const HashMap* map, void* key) {
    const size_t index = map->hash_func(key) % map->array_capacity;

    LinkedListNode* node_with_given_key = linked_list_search(&map->lists_with_nodes[index], key, find_existing_key);

    if (!node_with_given_key) {
        return NULL;
    }

    HashMapNode* node = (HashMapNode*)(node_with_given_key->data);

    return node->val;
}

void hash_map_clear(HashMap* map) {
    if (map->lists_with_nodes) {
        for (size_t i = 0; i < map->array_capacity; ++i) {
            linked_list_foreach(&map->lists_with_nodes[i], free);
            linked_list_clear(&map->lists_with_nodes[i]);
        }
    
        free(map->lists_with_nodes);
    }

    map->lists_with_nodes = NULL;
    map->array_capacity = 0;
    map->size = 0;
    map->hash_func = NULL;
}