#include "HashMap.h"
#include "../allocator.h"

void hash_map_init(HashMap* map, size_t(*hash_func)(const void*), bool(*are_keys_equal)(const void*, const void*)) {
    if (!map || !hash_func || !are_keys_equal) {
        return;
    }

    map->array_capacity = 512;
    map->size = 0;
    map->hash_func = hash_func;
    map->are_keys_equal = are_keys_equal;
    map->lists_with_nodes = (LinkedList*)allocate(map->array_capacity * sizeof(LinkedList));

    if (!(map->lists_with_nodes)) {
        return;
    }

    for (size_t i = 0; i < map->array_capacity; ++i) {
        linked_list_init(map->lists_with_nodes + i);
    }
}

static bool find_existing_key(const void* v_node, const void* key) {
    HashMapNode* node = (HashMapNode*)v_node;
    return node->are_keys_equal(node->key, key);
}

static void resize(HashMap* map, size_t new_capacity) {
    LinkedList* new_lists = (LinkedList*)allocate(new_capacity * sizeof(LinkedList));

    if (!new_lists) {
        return;
    }

    for (size_t i = 0; i < new_capacity; ++i) {
        linked_list_init(new_lists + i);
    }

    for (size_t i = 0; i < map->array_capacity; ++i) {
        LinkedListNode* list_node = map->lists_with_nodes[i].head;

        while(list_node) {
            HashMapNode* node = (HashMapNode*)(list_node->data);

            const size_t new_index = map->hash_func(node->key) % new_capacity;

            linked_list_push_back(&new_lists[new_index], node);

            list_node = list_node->next;
        }

        linked_list_clear(&map->lists_with_nodes[i]);
    }

    free(map->lists_with_nodes);

    map->array_capacity = new_capacity;
    map->lists_with_nodes = new_lists;
}

void hash_map_add(HashMap* map, void* key, void* val) {
    if (!map) {
        return;
    }

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

    if (map->size * 4 > 3 * map->array_capacity) {
        resize(map, map->array_capacity * 2);
    }
}

void* hash_map_get(const HashMap* map, const void* key) {
    if (!map) {
        return NULL;
    }

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