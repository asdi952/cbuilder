#include <pch.h>
#include <memory.h>
#include <stdlib.h>

#include "list.h"

void list_init(List * list){
    assert(list->state == List_state_uinit);
    list->state = List_state_init;

    list->capacity = 0;    
    list->count = 0;    
    list->data = NULL;
}

void reallocation(List *list, uint capacity){
    if(list->data == NULL){
        list->data = malloc(capacity * list->unitsize);
    }else{
        list->data = realloc(list->data, capacity * list->unitsize);
    }

    list->capacity = capacity;
}
 
void* list_get_ptr(List *list, uint index){
    assert(list->state == List_state_init);
    assert(index < list->count);

    return list->data + (index * list->unitsize);
}

void* _list_get_ptr(List *list, uint index){
    assert(list->state == List_state_init);
    assert(index < list->capacity);
    
    return list->data + (index * list->unitsize);
}

void list_get(List *list, uint index, void* dest){
    assert(list->state == List_state_init);
    assert(index < list->count);

    memcpy(dest, _list_get_ptr(list, index), list->unitsize);
}
void list_set(List *list, uint index, void* src){
    assert(list->state == List_state_init);
    assert(index < list->capacity);
    
    memcpy(_list_get_ptr(list, index), src, list->unitsize);
}

void list_push_back(List *list, void* src){
    assert(list->state == List_state_init);

    if(list->count == list->capacity){
        reallocation( list, list->capacity + 10);
    }

    list_set(list, list->count, src);

    list->count++;
}
int list_pop_back(List *list, void* dest){
    assert(list->state == List_state_init);

    if(list->count == 0) return 1;

    if(dest != NULL){
        list_get(list, list->count - 1, dest);
    }

    list->count--;
    return 0;
}
void list_remove(List *list, uint index, void* dest){
    assert(list->state == List_state_init);
    assert(index < list->count);

    if(dest != NULL){
        list_get(list, index, dest);
    }
    if(index < list->count - 1){
        list_set(list, index, _list_get_ptr(list, list->count -1));
    }
    list->count--;
}
void list_resize(List *list, uint capacity){
    assert(list->state == List_state_init);
    assert(list->count > capacity);

    reallocation(list, capacity);
}