#ifndef VECTOR_VOID_PTR
#define VECTOR_VOID_PTR

#include <stddef.h>

typedef struct Vector {
    size_t capacity;
    size_t size;
    void** data;
} Vector;

void vector_init(Vector* vector);
void vector_push(Vector* vector, void* data);
void vector_pop(Vector* vector);
void vector_free(Vector* vector);

#endif