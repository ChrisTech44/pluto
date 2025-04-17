#include <stdlib.h>
#include "list.h"

void list_init(List* list) {
    list->head.prev = NULL;
    list->tail.next = NULL;
    list->tail.prev = &list->head;
    list->head.next = &list->tail;
}

/*
* Removes an element from the list
* e = the element to remove
*/
void remove_element(Element* e) {
    e->prev->next = e->next;
    e->next->prev = e->prev;
}

/*
* Inserts an element at the end of a list
* e = the element to insert
*/
void insert_front(List* list, Element* e) {
    Element* head = &list->head;
    e->next = head->next;
    e->prev = head;
    head->next->prev = e;
    head->next = e;
}

/*
* Inserts an element at the end of a list
* e = the element to insert
*/
void insert_end(List* list, Element* e) {
    Element* tail = &list->tail;
    e->next = tail;
    e->prev = tail->prev;
    tail->prev->next = e;
    tail->prev = e;
}
