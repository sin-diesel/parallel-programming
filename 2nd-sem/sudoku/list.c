
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "list.h"

list_t* list_init() {

    list_t* list = (list_t*) calloc(1, sizeof(list_t));
    assert(list != NULL);

    list->data = 0;
    list->size = 0;
    list->next = NULL;
    list->prev = NULL;
    list->tail = list;

    return list;

}

void list_append(list_t* list, int data) {

    list->tail->next = (list_t*) calloc(1, sizeof(list_t));
    list->tail->next->prev = list->tail;
    list->tail = list->tail->next;
    list->tail->data = data;
    list->tail->next = NULL;
    list->size++;

}

void list_remove(list_t* list, int data) {
    while (list->next != NULL) {
        if (list->data == data) {
            list->prev->next = list->next;
            list->next->prev = list->prev;
            free(list);
            return;
        }
        list = list->next;
    }
}

void list_destruct(list_t* list) {

    while(list->next != NULL) {
        list = list->next;
        free(list->prev);
    }

    free(list);

}

void list_dump(list_t* list) {

    printf("list: %p\n", list);
    list_t* listIt = list;
    do {
        printf("data: %d\n", listIt->data);
        printf("list->next: %p\n", listIt->next);
        printf("list->prev: %p\n", listIt->prev);
        listIt = listIt->next;
    } while (listIt != NULL);
    
}
