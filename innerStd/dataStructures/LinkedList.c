#include "LinkedList.h"
#include "../allocator.h"

void linked_list_init(LinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

static void push_if_empty(LinkedList* list, void* data) {
    list->head = (LinkedListNode*)allocate(sizeof(LinkedListNode));
    list->tail = list->head;

    list->head->data = data;
    list->head->next = NULL;
}

static void push(LinkedList* list, void* data) {
    LinkedListNode* new_node = (LinkedListNode*)allocate(sizeof(LinkedListNode));

    new_node->data = data;
    new_node->next = NULL;

    list->tail->next = new_node;
    list->tail = new_node;
}

void linked_list_push_back(LinkedList* list, void* data) {
    if (linked_list_is_empty(list)) {
        push_if_empty(list, data);
    } else {
        push(list, data);
    }

    list->size++;
}

void linked_list_pop_back(LinkedList* list) {
    if (linked_list_is_empty(list)) {
        return;
    }

    if (list->head == list->tail) {
        deallocate(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return;
    }
    
    LinkedListNode* ptr = list->head;
    while (ptr->next != list->tail) {
        ptr = ptr->next;
    }
    
    deallocate(list->tail);
    list->tail = ptr;
    ptr->next = NULL;
    list->size--;
}

void linked_list_clear(LinkedList* list) {
    while (!linked_list_is_empty(list)) {
        linked_list_pop_back(list);
    }
}

bool linked_list_is_empty(const LinkedList* list) {
    return list->size == 0;
}

void linked_list_foreach(const LinkedList* list, void(*action)(void*)) {
    if (!list || !action) {
        return;
    }

    const LinkedListNode* ptr = list->head;
    while (ptr) {
        action(ptr->data);
        ptr = ptr->next;
    }
}

LinkedListNode* linked_list_search(const LinkedList* list, const void* to_find, bool(*are_equal)(const void*, const void*)) {
    if (!list || !are_equal) {
        return NULL;
    }

    LinkedListNode* ptr = list->head;
    while (ptr && !are_equal(ptr->data, to_find)) {
        ptr = ptr->next;
    }

    return ptr;
}
