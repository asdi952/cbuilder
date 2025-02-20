#include <pch.h>
#include <log.h>
#include <dlfcn.h>

#include "test.h"


pcchar sharedfile_callback_prefix = "UnitTestCallback";
int is_sharedfile_callback(pcchar fname){
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


int main(int argc, char** argv){
    mlog("Test Starting...\n");

    pcchar shlib_path = "../../bin/test_datastruct.so";

    void* shlib = dlopen( shlib_path, RTLD_LAZY);
    if(shlib == NULL){
        elog("fail to load shlib: \e\t %s\n", dlerror());
        return 1;
    }
    
    FORCOUNT(argc, i){
        if( is_sharedfile_callback(argv[i]) == 0){
            mlog("%s\n", argv[i]);

            UnitTestCallback fn = dlsym(shlib, argv[i]);
            if(fn == NULL){
                elog("fail to load fn %s\n", argv[i]);
                return 1;
            }

            UnitTestData dat;
            fn(&dat);
        }
    }
    mlog("Test Ending...\n");
    return 0;
}