#ifndef ALLOCATOR
#define ALLOCATOR

#include <stddef.h>

#define HEAP_CAPACITY (1<<23)

void init_heap(void);
void* allocate(size_t capacity);
void free(void* ptr);

#endif