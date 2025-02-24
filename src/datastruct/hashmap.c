#include <pch.h>
#include <stdlib.h>
#include <memory.h>

#include "list.h"
#include "hashmap.h"

static uint hash_function(void* data, uint size){
    uint step = sizeof(uint);
    uint i = 0;
    uint result = 0;
    while(1){
        if(i >= size){
            if( (i-size)%step != 0){
                uint holder = 0;
                holder = (*((uint*) data + i)) << (8*2);
                result += holder;
            }
            break;
        }

        result += *((uint*) data + i);
        i += step;
    }
    return result ;
}

void hashmap_init(Hashmap *hashmap, uint value_unitsize){
    assert(hashmap->state != Hashmap_state_init);
    hashmap->state = Hashmap_state_init;

    hashmap->hash_function = hash_function;

    hashmap->value_unitsize = value_unitsize;
    list_init(&hashmap->rows, sizeof(Hashmap_noderow));
    list_resize(&hashmap->rows, 200);

    FORLIST(&hashmap->rows, i){
        Hashmap_noderow* row = list_get_ptr(&hashmap->rows, i);
        list_init( &row->nodes, sizeof(Hashmap_node) + value_unitsize);
    }
}

void hashmap_deinit(Hashmap *hashmap){
    assert(hashmap->state == Hashmap_state_init);

    FORLIST(&hashmap->rows, i){
        Hashmap_noderow* row = list_get_ptr(&hashmap->rows, i);
        list_deinit(&row->nodes);
    }
    list_deinit(&hashmap->rows);
}



static inline int cmp_keys(void* key0, void* key1, uint size){
    FORCOUNT( size, i){
        if(((char*)key0)[i] != ((char*)key1)[i]){
            return 1;
        }
    }
    return 0;
}



void hashmap_add(Hashmap* hashmap, void* key_ptr, uint key_size, void* value_ptr){
    assert(hashmap->state == Hashmap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);
    assert(value_ptr != NULL);

    uint index = hashmap->hash_function(key_ptr, key_size) % hashmap->rows.count;
    
    Hashmap_noderow* row = list_get_ptr(&hashmap->rows, index) ;
    Hashmap_node* node =  list_emplace_back(&row->nodes);
    node->key_ptr = key_ptr;
    node->key_size = key_size;
    memcpy(node->value_ptr, value_ptr, hashmap->value_unitsize);
}

int hashmap_get(Hashmap* hashmap, void* key_ptr, uint key_size, void* dest){
    assert(hashmap->state == Hashmap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);
    assert(dest != NULL);

    uint index = hashmap->hash_function(key_ptr, key_size) % hashmap->rows.count;
    Hashmap_noderow* keyrow = list_get_ptr(&hashmap->rows, index);

    FORLIST(&keyrow->nodes, i){
        Hashmap_node* node = list_get_ptr( &keyrow->nodes, i);

        if(node->key_size != key_size) continue;
        if(cmp_keys(node->key_ptr, key_ptr, key_size)) continue;

        memcpy(dest, node->value_ptr, hashmap->value_unitsize);
        return 0;
    }

    return 1;
}
int hashmap_has(Hashmap* hashmap, void* key_ptr, uint key_size){
    assert(hashmap->state == Hashmap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);

    uint index = hashmap->hash_function(key_ptr, key_size) % hashmap->rows.count;
    Hashmap_noderow* keyrow = list_get_ptr(&hashmap->rows, index);

    FORLIST(&keyrow->nodes, i){
        Hashmap_node* node = list_get_ptr( &keyrow->nodes, i);

        if(node->key_size != key_size) continue;
        if(cmp_keys(node->key_ptr, key_ptr, key_size)) continue;

        return 0;
    }

    return 1;
}


