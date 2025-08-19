#include "StackVoidPtr.h"
#include "allocator.h"

void stack_void_ptr_init(StackVoidPtr* stack) {
    stack->size = 0;
    stack->capacity = 2;
    stack->data = (void**)allocate(stack->capacity * sizeof(void*));
}

static void resize(StackVoidPtr* stack, size_t new_capacity) {
    void** new_data = (void**)allocate(new_capacity * sizeof(void*));

    for (size_t i = 0; i < stack->size; ++i) {
        new_data[i] = stack->data[i];
    }

    free(stack->data);

    stack->data = new_data;
    stack->capacity = new_capacity;
}

void stack_void_ptr_push(StackVoidPtr* stack, void* data) {
    if (stack->size + 1 >= stack->capacity) {
        resize(stack, stack->capacity * 2);
    }

    stack->data[stack->size++] = data;
}

void stack_void_ptr_pop(StackVoidPtr* stack) {
    if (stack->size-- < stack->capacity / 2) {
        resize(stack, stack->capacity / 2);
    }
}

void* stack_void_ptr_top(const StackVoidPtr* stack) {
    return stack->data[stack->size - 1];
}

void stack_void_ptr_free(StackVoidPtr* stack) {
    stack->size = 0;
    stack->capacity = 0;
    free(stack->data);
}

void stack_void_ptr_foreach(StackVoidPtr* stack, void (*action)(void*)) {
    for (size_t i = 0; i < stack->size; ++i) {
        action(stack->data[i]);
    }
}
