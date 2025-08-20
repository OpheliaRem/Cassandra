#include "LinkedListVoidPtr.h"
#include "allocator.h"

void linked_list_void_ptr_init(LinkedListVoidPtr* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

static void push_if_empty(LinkedListVoidPtr* list, void* data) {
    list->head = (LinkedListVoidPtrNode*)allocate(sizeof(LinkedListVoidPtrNode));
    list->tail = list->head;

    list->head->data = data;
    list->head->next = NULL;
}

static void push(LinkedListVoidPtr* list, void* data) {
    LinkedListVoidPtrNode* new_node = (LinkedListVoidPtrNode*)allocate(sizeof(LinkedListVoidPtrNode));

    new_node->data = data;
    new_node->next = NULL;

    list->tail->next = new_node;
    list->tail = new_node;
}

void linked_list_void_ptr_push_back(LinkedListVoidPtr* list, void* data) {
    if (linked_list_void_ptr_is_empty(list)) {
        push_if_empty(list, data);
    } else {
        push(list, data);
    }

    list->size++;
}

void linked_list_void_ptr_pop_back(LinkedListVoidPtr* list) {
    if (linked_list_void_ptr_is_empty(list)) {
        return;
    }

    if (list->head == list->tail) {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return;
    }
    
    LinkedListVoidPtrNode* ptr = list->head;
    while (ptr->next != list->tail) {
        ptr = ptr->next;
    }
    
    free(list->tail);
    list->tail = ptr;
    ptr->next = NULL;
    list->size--;
}

void linked_list_void_ptr_clear(LinkedListVoidPtr* list) {
    while (!linked_list_void_ptr_is_empty(list)) {
        linked_list_void_ptr_pop_back(list);
    }
}

bool linked_list_void_ptr_is_empty(const LinkedListVoidPtr* list) {
    return list->size == 0;
}

void linked_list_void_ptr_foreach(const LinkedListVoidPtr* list, void(*action)(void*)) {
    if (!list || !action) return;

    const LinkedListVoidPtrNode* ptr = list->head;
    while (ptr) {
        action(ptr->data);
        ptr = ptr->next;
    }
}
