#include "Stack.h"
#include "../allocator.h"

void stack_init(Stack* stack) {
    stack->size = 0;
    stack->capacity = 2;
    stack->data = (void**)allocate(stack->capacity * sizeof(void*));
}

static void resize(Stack* stack, size_t new_capacity) {
    void** new_data = (void**)allocate(new_capacity * sizeof(void*));

    for (size_t i = 0; i < stack->size; ++i) {
        new_data[i] = stack->data[i];
    }

    free(stack->data);

    stack->data = new_data;
    stack->capacity = new_capacity;
}

void stack_push(Stack* stack, void* data) {
    if (stack->size + 1 >= stack->capacity) {
        resize(stack, stack->capacity * 2);
    }

    stack->data[stack->size++] = data;
}

void stack_pop(Stack* stack) {
    if (stack->size-- < stack->capacity / 2) {
        resize(stack, stack->capacity / 2);
    }
}

void* stack_top(const Stack* stack) {
    return stack->data[stack->size - 1];
}

void stack_free(Stack* stack) {
    stack->size = 0;
    stack->capacity = 0;
    free(stack->data);
}

void stack_foreach(Stack* stack, void (*action)(void*)) {
    for (size_t i = 0; i < stack->size; ++i) {
        action(stack->data[i]);
    }
}

bool stack_is_empty(const Stack* stack) {
    return stack->size == 0;
}
