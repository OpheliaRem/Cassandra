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
Vector vector_copy(const Vector* vector);
Vector vector_move(Vector* vector);
bool vector_is_invalid(const Vector* vector);
bool vector_is_empty(const Vector* vector);
void vector_push(Vector* vector, void* data);
void vector_pop(Vector* vector);
void vector_foreach(Vector* vector, void(*action)(void*));
void vector_immutable_foreach(const Vector* vector, void(*action)(const void*));
void* vector_get(const Vector* vector, size_t index);
const void* vector_immutable_get(const Vector* vector, size_t index);
void vector_set(Vector* vector, size_t index, void* val);
void vector_qsort(Vector* vector, bool(*first_is_larger)(void*, void*));
long vector_binary_search(const Vector* vector, void* key, bool(*are_equal)(void*, void*), bool(*first_is_larger)(void*, void*));
bool vector_contains(const Vector* vector, void* key, bool(*are_equal)(void*, void*), bool(*first_is_larger)(void*, void*));

#endif