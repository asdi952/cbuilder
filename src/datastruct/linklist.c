#include "defines.h"
#include <pch.h>
#include <memory.h>


DEFINE_STRUCT(LinkListMod, 
    void* next;
    void* data;
);

DEFINE_ENUM(LinkList_state,
    LinkList_state_uint,
    LinkList_state_init,
);

DEFINE_STRUCT(LinkList,
    LinkList_state state;
    uint linklistId;

    LinkListMod* start;
    LinkListMod* end;
    uint count;
);
//-------------------------------------

UNIQUEID

void linklist_init(LinkList* list){
    assert(list->state != LinkList_state_init);
    list->state = LinkList_state_init;
    list->linklistId = getUniqueId();

    list->start = NULL;
    list->end = NULL;
    list->count = 0;
}

void linklist_push_back(LinkList* list, void* src, uint size){
    assert(list->state == LinkList_state_init);
    assert(src != NULL);

    LinkListMod* mod;
    memcpy(mod->data, src, size);

    if(list->count == 0){
        list->start = list->end = mod;
    }
    else{
        list->end->next = mod;
        list->end = mod;
    }

    list->count++;
}

