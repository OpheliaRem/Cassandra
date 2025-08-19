#include "VectorVoidPtr.h"
#include "allocator.h"

void vector_void_ptr_init(VectorVoidPtr* vector) {
    vector->size = 0;
    vector->capacity = 2;
    vector->data = (void**)allocate(vector->capacity * sizeof(void*));
}

static void resize(VectorVoidPtr* vector, size_t new_capacity) {
    void** new_data = (void**)allocate(new_capacity * sizeof(void*));

    for (size_t i = 0; i < vector->size; ++i) {
        new_data[i] = vector->data[i];
    }

    free(vector->data);

    vector->data = new_data;
    vector->capacity = new_capacity;
}

void vector_void_ptr_push(VectorVoidPtr* vector, void* data) {
    if (vector->size + 1 >= vector->capacity) {
        resize(vector, vector->capacity * 2);
    }

    vector->data[vector->size++] = data;
}

void vector_void_ptr_pop(VectorVoidPtr* vector) {
    if (vector->size-- < vector->capacity / 2) {
        resize(vector, vector->capacity / 2);
    }
}

void vector_void_ptr_free(VectorVoidPtr* vector) {
    vector->size = 0;
    vector->capacity = 0;
    free(vector->data);
}
