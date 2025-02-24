#pragma once
#include "defines.h"
#include "types.h"
#include <pch.h>
#include <list.h>


DEFINE_STRUCT(Unittest_data, 
    int result;
    pcchar description;
);
DEFINE_STRUCT(Unittest_test, 
    List datas;
   
);

DEFINE_FUNCTION_TYPE(Unittest_callback, void, (Unittest_test*));
DEFINE_FUNCTION_TYPE(Unittest_callback_get, void, (pcchar* testname, pcchar* groupname, pcchar* description, Unittest_callback* callback));


#define UNITTEST(GROUPNAME, TESTNAME, DESCRIPTION, BODY)\
    void Unittest_callback_##GROUPNAME##_##TESTNAME(Unittest_test* test){\
        BODY\
    }\
    void Unittest_callback_get_##GROUPNAME##_##TESTNAME(pcchar* testname, pcchar* groupname, pcchar* description, Unittest_callback* callback){\
        *testname = #TESTNAME;\
        *groupname = #GROUPNAME;\
        *description = #DESCRIPTION;\
        *callback = Unittest_callback_##GROUPNAME##_##TESTNAME;\
    }







