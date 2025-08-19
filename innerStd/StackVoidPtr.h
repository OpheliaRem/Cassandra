#ifndef STACK_VOID_PTR
#define STACK_VOID_PTR

#include <stddef.h>
#include <stdbool.h>

typedef struct StackVoidPtr {
    size_t size;
    size_t capacity;
    void** data;
} StackVoidPtr;

void stack_void_ptr_init(StackVoidPtr* stack);
void stack_void_ptr_push(StackVoidPtr* stack, void* data);
void stack_void_ptr_pop(StackVoidPtr* stack);
void* stack_void_ptr_top(const StackVoidPtr* stack);
void stack_void_ptr_free(StackVoidPtr* stack);
void stack_void_ptr_foreach(StackVoidPtr* stack, void(*action)(void*));
bool stack_void_ptr_is_empty(const StackVoidPtr* stack);

#endif