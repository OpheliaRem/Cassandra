#include "allocator.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint8_t alloc_heap[HEAP_CAPACITY];
static bool alloc_bitmap[HEAP_CAPACITY];

void initialize_heap() {
    for (int i = 0; i < HEAP_CAPACITY; ++i) {
        alloc_bitmap[i] = false;
    }
}

void* allocate(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size_t index = 0;
    while (index < HEAP_CAPACITY) {
       
        while (index < HEAP_CAPACITY && alloc_bitmap[index]) {
            index++;
        }
        
        size_t start = index;
        if (start >= HEAP_CAPACITY || HEAP_CAPACITY - start < size + 1) {
            return NULL; // Not enough space
        };

        bool space_found = true;
        for (size_t i = 0; i < size; i++) {
            if (alloc_bitmap[start + i]) {
                space_found = false;
                index = start + i + 1;
                break;
            }
        }

        if (!space_found) {
            continue;
        }

        for (size_t i = 0; i < size; i++) {
            alloc_bitmap[start + i] = true;
        }
        
        alloc_bitmap[start + size] = false;

        return &alloc_heap[start];
    }

    return NULL;
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    size_t index = (uint8_t*)ptr - alloc_heap;
    if (index >= HEAP_CAPACITY) {
        return;
    }

    for (size_t i = index; i < HEAP_CAPACITY && alloc_bitmap[i]; i++) {
        alloc_bitmap[i] = false;
    }
}