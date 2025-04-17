#include<stddef.h>
#define get_entry(ELEMENT, STRUCT, MEMBER) \
    ((STRUCT*) ((char*) ELEMENT)           \
        - offsetof(STRUCT, MEMBER))

typedef struct list_element {
    struct list_element* prev; /*Previous element*/
    struct list_element* next; /*Next element*/
} Element;

typedef struct free_list {
    Element head; /*Dummy head*/
    Element tail; /*Dummy tail*/
} List; 

void list_init(List* list);
void remove_element(Element* e);
void insert_front(List* list, Element* e);
void insert_end(List* list, Element* e);
// void list_start(List* list);
// void list_end(List* list);