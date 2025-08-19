#ifndef VECTOR_VOID_PTR
#define VECTOR_VOID_PTR

#include <stddef.h>

typedef struct VectorVoidPtr {
    size_t capacity;
    size_t size;
    void** data;
} VectorVoidPtr;

void vector_void_ptr_init(VectorVoidPtr* vector);
void vector_void_ptr_push(VectorVoidPtr* vector, void* data);
void vector_void_ptr_pop(VectorVoidPtr* vector);
void vector_void_ptr_free(VectorVoidPtr* vector);

#endif