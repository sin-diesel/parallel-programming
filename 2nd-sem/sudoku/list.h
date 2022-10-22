#pragma once

typedef struct list_t list_t;


struct list_t {
    
    int data;
    list_t* next;
    list_t* prev;
    int size;

    list_t* tail;

};

list_t* list_init();

void list_append(list_t* list, int data);

void list_destruct(list_t* list);

void list_remove(list_t* list, int data);

void list_dump(list_t* list);
