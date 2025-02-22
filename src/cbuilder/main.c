#include "defines.h"
#include <pch.h>
#include <log.h>
#include <list.h>
#include <hashmap.h>
#include <string.h>

int main(){
    mlog("Cbuilder starting...\n");
    
    // List list LIST_GENERIC(uint);
    // list_init(&list);

    // FORCOUNT(20, i){
    //     uint* val = list_emplace_back(&list);
    //     *val = i;
    // }

    // FORLIST(&list, i){
    //     uint aux;
    //     list_get(&list, i, &aux);
    //     mlog("%d\n", aux);
    // }
    pcchar key;
    
    #define a(key) mlog("%ud\n", hash_function((void*)key, strlen(key)));
    
    a("coocnut");
    a("c asdas doocnut");
    a("coocnuas d ast");

    
    mlog("Cbuilder ending...\n");
    return 0;
}