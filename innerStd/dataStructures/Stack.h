#ifndef STACK_VOID_PTR
#define STACK_VOID_PTR

#include <stddef.h>
#include <stdbool.h>

typedef struct Stack {
    size_t size;
    size_t capacity;
    void** data;
} Stack;

void stack_init(Stack* stack);
void stack_push(Stack* stack, void* data);
void stack_pop(Stack* stack);
void* stack_top(const Stack* stack);
void stack_free(Stack* stack);
void stack_foreach(Stack* stack, void(*action)(void*));
bool stack_is_empty(const Stack* stack);

#endif