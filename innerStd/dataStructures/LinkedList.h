#ifndef LINKED_LIST_VOID_PTR
#define LINKED_LIST_VOID_PTR

#include <stddef.h>
#include <stdbool.h>

typedef struct LinkedListNode {
    void* data;
    struct LinkedListNode* next;
} LinkedListNode;

typedef struct LinkedList {
    LinkedListNode* head;
    LinkedListNode* tail;
    size_t size;
} LinkedList;

void linked_list_init(LinkedList* list);
void linked_list_push_back(LinkedList* list, void* data);
void linked_list_pop_back(LinkedList* list);
void linked_list_clear(LinkedList* list);
bool linked_list_is_empty(const LinkedList* list);
void linked_list_foreach(const LinkedList* list, void(*action)(void*));

#endif