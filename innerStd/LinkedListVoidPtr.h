#ifndef LINKED_LIST_VOID_PTR
#define LINKED_LIST_VOID_PTR

#include <stddef.h>
#include <stdbool.h>

typedef struct LinkedListVoidPtrNode {
    void* data;
    struct LinkedListVoidPtrNode* next;
} LinkedListVoidPtrNode;

typedef struct LinkedListVoidPtr {
    LinkedListVoidPtrNode* head;
    LinkedListVoidPtrNode* tail;
    size_t size;
} LinkedListVoidPtr;

void linked_list_void_ptr_init(LinkedListVoidPtr* list);
void linked_list_void_ptr_push_back(LinkedListVoidPtr* list, void* data);
void linked_list_void_ptr_pop_back(LinkedListVoidPtr* list);
void linked_list_void_ptr_clear(LinkedListVoidPtr* list);
bool linked_list_void_ptr_is_empty(const LinkedListVoidPtr* list);
void linked_list_void_ptr_foreach(const LinkedListVoidPtr* list, void(*action)(void*));

#endif