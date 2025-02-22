#include "defines.h"
#include <pch.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <memory.h>

DEFINE_FUNCTION_TYPE(HashFunction, uint, (void* data, uint size));

DEFINE_ENUM(HashMap_state, 
    HashMap_state_uinit,
    HashMap_state_init,
);

DEFINE_STRUCT( HashMapNode, 
    void* key_ptr;
    uint key_size;

    void* value_ptr[];
);

DEFINE_STRUCT( HashMapNodeRow,
    List(HashMapNode) nodes;
);

DEFINE_STRUCT( HashMap, 
    HashMap_state state;

    uint value_unitsize;

    HashFunction hash_function;
    List(HashMapNodeRow) row;
);


uint hash_function(void* data, uint size){
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

void hashmap_init(HashMap *hashmap, uint value_unitsize){
    assert(hashmap->state != HashMap_state_init);
    hashmap->state = HashMap_state_init;

    hashmap->value_unitsize = value_unitsize;
    list_init(&hashmap->row, sizeof(HashMapNodeRow));
    list_resize(&hashmap->row, 200);

    FORLIST(&hashmap->row, i){
        HashMapNodeRow* row = list_get_ptr(&hashmap->row, i);
        list_init( &row->nodes, sizeof(HashMapNode) + value_unitsize);
    }
}


static inline int cmp_keys(void* key0, void* key1, uint size){
    FORCOUNT( size, i){
        if(((char*)key0)[i] != ((char*)key1)[i]){
            return 1;
        }
    }
    return 0;
}



void hashmap_add(HashMap* hashmap, void* key_ptr, uint key_size, void* value_ptr){
    assert(hashmap->state == HashMap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);
    assert(value_ptr != NULL);

    uint index = hash_function(key_ptr, key_size) % hashmap->row.count;
    
    HashMapNodeRow* row = list_get_ptr(&hashmap->row, index) ;
    HashMapNode* node =  list_emplace_back(&row->nodes);
    node->key_ptr = key_ptr;
    node->key_size = key_size;
    memcpy(node->value_ptr, value_ptr, hashmap->value_unitsize);
}

int hashmap_get(HashMap* hashmap, void* key_ptr, uint key_size, void* dest){
    assert(hashmap->state == HashMap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);
    assert(dest != NULL);

    uint index = hash_function(key_ptr, key_size) % hashmap->row.count;
    HashMapNodeRow* keyrow = list_get_ptr(&hashmap->row, index);

    FORLIST(&keyrow->nodes, i){
        HashMapNode* node = list_get_ptr( &keyrow->nodes, i);

        if(node->key_size != key_size) continue;
        if(cmp_keys(node->key_ptr, key_ptr, key_size)) continue;

        memcpy(dest, node->value_ptr, hashmap->value_unitsize);
        return 0;
    }

    return 1;
}
int hashmap_has(HashMap* hashmap, void* key_ptr, uint key_size){
    assert(hashmap->state == HashMap_state_init);
    assert(key_ptr != NULL);
    assert(key_size != 0);

    uint index = hash_function(key_ptr, key_size) % hashmap->row.count;
    HashMapNodeRow* keyrow = list_get_ptr(&hashmap->row, index);

    FORLIST(&keyrow->nodes, i){
        HashMapNode* node = list_get_ptr( &keyrow->nodes, i);

        if(node->key_size != key_size) continue;
        if(cmp_keys(node->key_ptr, key_ptr, key_size)) continue;

        return 0;
    }

    return 1;
}

int main(){
    HashMap map;
    pcchar key;
    int value;

    hashmap_init(&map, sizeof(int));
    #define add(_key, _value) \
        value = _value;\
        hashmap_add(&map, _key, strlen(_key), &value)

    add("coco", 223);
    add("hihi", 6);
    add("hisdhi", 66);
    add("hi", 3);

    // FORLIST(&map.row, i){
    //     HashMapNodeRow* row =  list_get_ptr(&map.row, i);
    //     FORLIST(&row->nodes, k){
    //         int* value = list_get_ptr(&row->nodes, k);
    //         mlog("row: %d index %d value %d\n", i, k, *value);
    //     }
    // }

    int res;
    #define get(_key)\
        res = hashmap_get(&map, _key, strlen(_key), &value);\
        if(res == 0) mlog("%d\n", value);\
        else mlog("key , is not defined\n");
        
    get("coco");
    get("hihi");
    get("hisdhi");
    get("non");

    #define has(_key)\
        res = hashmap_has(&map, _key, strlen(_key));\
        if(res == 0) mlog("has key %s\n", _key);\
        else mlog("does not have key %s\n", _key)

    has("coconut");
    has("hihi");

    


    return 0;
}

