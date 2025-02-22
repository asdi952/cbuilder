#include <pch.h>
#include <log.h>
#include <dlfcn.h>
#include <pthread.h>
#include <list.h>

#include "test.h"


List test_callbacks LIST_GENERIC(UnitTestCallback);

pcchar sharedfile_callback_prefix = "UnitTestCallback";
int is_sharedfile_callback(pcchar fname)
{
    uint i = 0;
    while(1){
        char ch0 = fname[i];
        char ch1 = sharedfile_callback_prefix[i];

        if(ch1 == '\0'){ break;}
        else if(ch0 == '\0'){ return 1;}
        else if( ch0 != ch1){ return 1;}

        i++;
    }
    return 0;
}

int load_sharedlib(pcchar path, pcchar* args, int count)
{
    void* shlib = dlopen( path, RTLD_LAZY);
    if(shlib == NULL){
        elog("fail to load shlib: \e\t %s\n", dlerror());
        return 1;
    }

    FORCOUNT(count, i){
        if( is_sharedfile_callback(args[i]) == 0){
            UnitTestCallback fn = dlsym( shlib, args[i]);
            if(fn == NULL){
                elog("fail to load fn %s\n", args[i]);
                continue;
            }
            list_push_back(&test_callbacks, &fn);
        }
    }
    return 0;
}

void run_all_tests()
{
    FORLIST(&test_callbacks, i){
        UnitTestCallback fn;
        list_get(&test_callbacks, i, &fn);
        
        UnitTestTest test;
        fn(&test);
    }
}


int main(int argc, char const** argv)
{
    mlog("Test Starting...\n");

    list_init(&test_callbacks);

    pcchar shlib_path = "../../bin/test_datastruct.so";
    if(load_sharedlib(shlib_path, argv, argc) != 0) return 1;
    
    run_all_tests();

    mlog("Test Ending...\n");
    return 0;
}