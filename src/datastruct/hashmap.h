#include <pch.h>
#include "list.h"
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


uint hash_function(void* data, uint size);
void hashmap_init(HashMap *hashmap, uint value_unitsize);



void hashmap_add(HashMap* hashmap, void* key_ptr, uint key_size, void* value_ptr);
int hashmap_get(HashMap* hashmap, void* key_ptr, uint key_size, void* dest);


