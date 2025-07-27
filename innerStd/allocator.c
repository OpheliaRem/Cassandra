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

void* allocate(size_t capacity) {
    size_t index = 0;
    while (index < HEAP_CAPACITY && alloc_bitmap[index]) {
        ++index;
    }
    ++index; //acknowledge offset one byte to separate allocated areas

    if (index >= HEAP_CAPACITY || HEAP_CAPACITY - index < capacity + 1) {
        return NULL;
    }

    for (size_t i = index; i < capacity; ++i) {
        alloc_bitmap[i] = true;
    }
    alloc_bitmap[capacity] = false; //make offset one byte to separate allocated areas

    return &alloc_heap[index];
}

void free(void* ptr) {
    size_t index = (uint8_t*)ptr - alloc_heap;

    if (index >= HEAP_CAPACITY) {
        return;
    }

    for (size_t i = index; alloc_bitmap[i] && i < HEAP_CAPACITY; ++i) {
        alloc_bitmap[i] = false;
    }
}