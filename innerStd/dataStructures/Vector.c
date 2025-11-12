#include "Vector.h"
#include "../allocator.h"
#include <stdarg.h>

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

    deallocate(vector->data);
    vector->data = new_data;
    vector->capacity = new_capacity;
}

Vector vector_new() {
    Vector res;
    vector_init(&res);
    return res;
}

Vector vector_copy(const Vector* other) {
    Vector res;
    res.capacity = other->capacity;
    res.size = other->size;
    res.data = allocate(res.capacity * sizeof(void*));

    for (size_t i = 0; i < res.size; ++i) {
        res.data[i] = other->data[i];
    }

    return res;
}

Vector vector_move(Vector* other) {
    Vector res;
    res.capacity = other->capacity;
    res.size = other->size;
    res.data = other->data;

    other->size = 0;
    other->capacity = 0;
    other->data = NULL;

    return res;
}

bool vector_is_invalid(const Vector* vector) {
    return vector->capacity == 0 || vector->data == NULL;
}

bool vector_is_empty(const Vector* vector) {
    return vector->size == 0;
}

void vector_push(Vector* vector, void* data) {
    if (vector_is_invalid(vector)) {
        return;
    }

    if (vector->size == vector->capacity) {
        resize(vector, vector->capacity << 1);
    }

    vector->data[vector->size++] = data;
}

void vector_pop(Vector* vector) {
    if (vector_is_invalid(vector) || vector->size == 0) {
        return;
    }

    vector->size--;

    if (vector->capacity > 2 && vector->size <= vector->capacity / 4) {
        resize(vector, vector->capacity / 2);
    }
}

void vector_free(Vector* vector) {
    if (vector_is_invalid(vector)) {
        return;
    }

    deallocate(vector->data);
    vector->size = 0;
    vector->capacity = 0;
    vector->data = NULL;
}

void vector_foreach(Vector* vector, void(*action)(void*)) {
    for (size_t i = 0; i < vector->size; ++i) {
        action(vector->data[i]);
    }
}

void vector_immutable_foreach(const Vector* vector, void(*action)(const void*)) {
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

const void* vector_immutable_get(const Vector* vector, size_t index) {
    return vector_get(vector, index);
}

void vector_set(Vector* vector, size_t index, void* val) {
    if (index >= vector->size) {
        return;
    }

    vector->data[index] = val;
}

static inline void swap(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void qsort(void** array, size_t size, bool(*first_is_larger)(void*, void*)) {
    if (size < 2) {
        return;
    }

    size_t pivot_index = size / 2;

    size_t i = 0;
    while (i < pivot_index) {
        if (first_is_larger(array[i], array[pivot_index])) {
            if (pivot_index - i != 1) {
                swap(&array[pivot_index], &array[pivot_index - 1]);
            }
            swap(&array[pivot_index], &array[i]);
            --pivot_index;
        } else {
            ++i;
        }
    }

    i = size - 1;
    while (i > pivot_index) {
        if (!first_is_larger(array[i], array[pivot_index])) {
            if (i - pivot_index != 1) {
                swap(&array[pivot_index], &array[pivot_index + 1]);
            }
            swap(&array[pivot_index], &array[i]);
            ++pivot_index;
        } else {
            --i;
        }
    }

    qsort(array, pivot_index, first_is_larger);
    qsort(&array[pivot_index], size - pivot_index, first_is_larger);
}

void vector_qsort(Vector* vector, bool(*first_is_larger)(void*, void*)) {
    qsort(vector->data, vector->size, first_is_larger);
}

long vector_binary_search(const Vector* vector, void* key, bool(*are_equal)(void*, void*), bool(*first_is_larger)(void*, void*)) {
    size_t left, mid, right;
    left = 0;
    right = vector->size;
    mid = (right - left) / 2;
    
    while (right - left > 0) {
        if (are_equal(vector->data[mid], key)) {
            return (long)mid;
        }

        if (first_is_larger(key, vector->data[mid])) {
            left = mid + 1;
        } else {
            right = mid;
        }

        mid = left + (right - left) / 2;
    }

    return -1l;
}

bool vector_contains(const Vector* vector, void* key, bool(*are_equal)(void*, void*), bool(*first_is_larger)(void*, void*)) {
    return vector_binary_search(vector, key, are_equal, first_is_larger) != -1;
}
