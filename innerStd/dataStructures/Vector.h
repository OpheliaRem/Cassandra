#ifndef VECTOR_VOID_PTR
#define VECTOR_VOID_PTR

#include <stddef.h>
#include <stdbool.h>

typedef struct Vector {
    size_t capacity;
    size_t size;
    void** data;
} Vector;

void vector_init(Vector* vector);
void vector_free(Vector* vector);
Vector vector_new();
bool vector_is_empty(const Vector* vector);
void vector_push(Vector* vector, void* data);
void vector_pop(Vector* vector);
void vector_foreach(Vector* vector, void(*action)(void*));
void* vector_get(const Vector* vector, size_t index);

#endif