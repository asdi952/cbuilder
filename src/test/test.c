#include <pch.h>
#include <log.h>
#include <dlfcn.h>
#include <list.h>
#include <hashmap.h>
#include <bstring.h>

#include "test.h"
#include "defines.h"
#include "error.h"
#include "types.h"



pcchar sharedfile_callback_get_prefix = "UnitTestCallback_get";
int is_sharedfile_callback_get(pcchar fname)
{
    uint i = 0;
    while(1){
        char ch0 = fname[i];
        char ch1 = sharedfile_callback_get_prefix[i];

        if(ch1 == '\0'){ break;}
        else if(ch0 == '\0'){ return 1;}
        else if( ch0 != ch1){ return 1;}

        i++;
    }
    return 0;
}

DEFINE_STRUCT(MemoryPool, 
    List(MemoryBlock) blocks;
);


DEFINE_ENUM(MemoryBlockType,
    MemoryBlockType_page,
);

DEFINE_ENUM(MemoryBlock_state,
    MemoryBlock_state_uinit,
    MemoryBlock_state_init,
);

DEFINE_STRUCT(MemoryBlock, 
    MemoryBlock_state state;

    MemoryBlockType type;
    void* data[];
);

#include <unistd.h>
#include <sys/mman.h>


DEFINE_STRUCT(SystemMemory,

);

void memorypool_init(MemoryPool* pool){
    list_init( &pool->blocks, sizeof(MemoryBlock*));
}
int create_page(MemoryPool* pool){

    size_t pagesize = sysconf(_SC_PAGESIZE);
    void *page = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, 
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (page == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    
    list_emplace_back( &pool->blocks);




    return 0;
}

DEFINE_STRUCT(Unittest, 
    Bstring testname;
    Bstring groupname;
    Bstring description;
);

DEFINE_STRUCT(Unittest_registry, 
    Hashmap(Unittest) testnames;
    Hashmap(Unittest) testgroups;
    
    List(Unittest) alltests;
);

Unittest_registry registry;



int load_sharedlib(pcchar path, pcchar* args, int count)
{
    void* shlib = dlopen( path, RTLD_LAZY);
    if(shlib == NULL){
        errorset_fmt("fail to load shlib: \e\t %s\n", dlerror());
        mlog("faile to opemn\n");
        return 1;
    }

    FORCOUNT(count, i){
        if( is_sharedfile_callback_get(args[i]) == 0){
            Unittest_callback_get fn = dlsym( shlib, args[i]);
            if(fn == NULL){
                errorset_fmt("fail to load fn %s\n", args[i]);
                continue;
            }

            pcchar testname;
            pcchar groupname;
            pcchar description;
            Unittest_callback callback;
            fn(&testname, &groupname, &description, &callback);

            mlog( "%s %s %s \n", testname, groupname, description);


        }
    }
    return 0;
}

void run_all_tests()
{
    // FORLIST(&test_callbacks, i){
    //     UnitTestCallback fn;
    //     list_get(&test_callbacks, i, &fn);
        
    //     UnitTestTest test;
    //     fn(&test);
    // }
}

int init(){
    error_init();
    
    hashmap_init(&registry.testgroups, sizeof(Unittest));
    hashmap_init(&registry.testnames, sizeof(Unittest));
    list_init(&registry.alltests, sizeof(Unittest));
    return 0;
}

int deinit(){
    list_deinit(&registry.alltests);
    hashmap_deinit(&registry.testnames);
    hashmap_deinit(&registry.testgroups);
    error_deinit();
    return 0;
}


int app(int argc, char const** argv){
    if(init() != 0) {errorset_pcchar("Error init"); return 1;}

    pcchar shlib_path = "../../bin/test_datastruct.so";
    if(load_sharedlib(shlib_path, argv, argc) != 0) {return 1;}
    
    run_all_tests();

    if(deinit() != 0) {errorset_pcchar("Error deinit"); return 1;}

    return 0;
}


int main(int argc, char const** argv)
{
    mlog("Test Starting...\n");
    
    if(app(argc, argv) != 0){
        term_print_error_stack();
    }
   
    mlog("Test Ending...\n");
    return 0;
}