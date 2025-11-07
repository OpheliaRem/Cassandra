#include "Vector.h"
#include "../allocator.h"

#include "../../vgaBufferTerminal/terminal.h"

void vector_init(Vector* vector) {
    vector->size = 0;
    vector->capacity = 2;
    vector->data = (void**)allocate(vector->capacity * sizeof(void*));
}

static void resize(Vector* vector, size_t new_capacity) {
    if (new_capacity < 2) {
        new_capacity = 2;
    }

    void** new_data = (void**)allocate(new_capacity * sizeof(void*));
    if (!new_data) {
        return;
    }

    for (size_t i = 0; i < vector->size; ++i) {
        new_data[i] = vector->data[i];
    }

    free(vector->data);
    vector->data = new_data;
    vector->capacity = new_capacity;
}

Vector vector_new() {
    Vector res;
    vector_init(&res);
    return res;
}

bool vector_is_empty(const Vector* vector) {
    return vector->size == 0;
}

void vector_push(Vector* vector, void* data) {
    if (vector->size == vector->capacity) {
        resize(vector, vector->capacity << 1);
    }

    vector->data[vector->size++] = data;
}

void vector_pop(Vector* vector) {
    if (vector->size == 0) {
        return;
    }
    vector->size--;

    if (vector->capacity > 2 && vector->size <= vector->capacity / 4) {
        resize(vector, vector->capacity / 2);
    }
}

void vector_free(Vector* vector) {
    free(vector->data);
    vector->size = 0;
    vector->capacity = 0;
    vector->data = NULL;
}

void vector_foreach(Vector* vector, void(*action)(void*)) {
    for (size_t i = 0; i < vector->size; ++i) {
        action(vector->data[i]);
    }
}

void* vector_get(const Vector* vector, size_t index) {
    if (index >= vector->size) {
        return NULL;
    }

    return vector->data[index];
}
