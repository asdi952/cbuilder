#pragma once
#include <pch.h>

DEFINE_ENUM(List_State,
    List_state_uinit,
    List_state_init,
);
DEFINE_STRUCT(List, 
    List_State state;
    const uint unitsize;

    uint capacity;
    uint count;
    void* data;
);

#define LIST_GENERIC(TYPE) = { .unitsize = sizeof(TYPE), .state = List_state_uinit};

#define FORLIST(LISTP, i) for(uint i = 0; i < (LISTP)->count; i++)

void list_init(List * list);

void* list_get_ptr(List *list, uint index);
void list_get(List *list, uint index, void* dest);

void list_set(List *list, uint index, void* src);

void list_push_back(List *list, void* src);
int list_pop_back(List *list, void* dest);

void list_remove(List *list, uint index, void* dest);

void list_resize(List *list, uint capacity);