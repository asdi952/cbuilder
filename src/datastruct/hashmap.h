#include <pch.h>
#include "list.h"

DEFINE_FUNCTION_TYPE(Hash_function, uint, (void* data, uint size));

DEFINE_ENUM(Hashmap_state, 
    Hashmap_state_uinit = 10,
    Hashmap_state_init,
);

DEFINE_STRUCT( Hashmap_node, 
    void* key_ptr;
    uint key_size;

    void* value_ptr[];
);

DEFINE_STRUCT( Hashmap_noderow,
    List(Hashmap_node) nodes;
);

DEFINE_STRUCT( Hashmap, 
    Hashmap_state state;

    uint value_unitsize;

    Hash_function hash_function;
    List(Hashmap_noderow) rows;
);

#define Hashmap(TYPE) Hashmap

void hashmap_init(Hashmap *hashmap, uint value_unitsize);
void hashmap_deinit(Hashmap *hashmap);

void hashmap_add(Hashmap* hashmap, void* key_ptr, uint key_size, void* value_ptr);
int hashmap_get(Hashmap* hashmap, void* key_ptr, uint key_size, void* dest);

int hashmap_has(Hashmap* hashmap, void* key_ptr, uint key_size);

