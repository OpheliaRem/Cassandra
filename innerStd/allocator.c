#include "allocator.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint8_t alloc_heap[HEAP_CAPACITY];
static bool alloc_bitmap[HEAP_CAPACITY];

typedef struct {
    size_t size;
} BlockHeader;

void init_heap() {
    for (int i = 0; i < HEAP_CAPACITY; ++i) {
        alloc_bitmap[i] = false;
    }
}

static inline void bitmap_set(size_t start, size_t len, bool value) {
    for (size_t i = start; i < start + len && i < HEAP_CAPACITY; i++) {
        alloc_bitmap[i] = value;
    }
}

void* allocate(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size_t total = sizeof(BlockHeader) + size;

    for (size_t start = 0; start + total <= HEAP_CAPACITY; ++start) {
        bool space_found = true;

        for (size_t i = 0; i < total; ++i) {
            if (alloc_bitmap[start + i]) {
                space_found = false;
                start += i;
                break;
            }
        }

        if (!space_found) {
            continue;
        }

        bitmap_set(start, total, true);

        BlockHeader* header = (BlockHeader*)&alloc_heap[start];
        header->size = size;

        return (void*)(header + 1);
    }

    return NULL;
}

void deallocate(void* ptr) {
    if (!ptr) {
        return;
    }

    BlockHeader* header = ((BlockHeader*)ptr) - 1;

    size_t start = (uint8_t*)header - alloc_heap;
    if (start >= HEAP_CAPACITY) {
        return;
    }

    size_t total = sizeof(BlockHeader) + header->size;
    bitmap_set(start, total, false);
}