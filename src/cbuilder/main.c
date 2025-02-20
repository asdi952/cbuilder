#include "defines.h"
#include <pch.h>
#include <log.h>
#include <list.h>

int main(){
    mlog("Cbuilder starting...\n");
    
    List list LIST_GENERIC(uint);
    list_init(&list);

    FORCOUNT(20, i){
        list_push_back(&list, &i);
    }

    FORLIST(&list, i){
        uint aux;
        list_get(&list, i, &aux);
        mlog("%d\n", aux);
    }

    mlog("Cbuilder ending...\n");
    return 0;
}