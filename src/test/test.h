#pragma once
#include "defines.h"
#include "types.h"
#include <pch.h>
#include <list.h>

DEFINE_STRUCT(UnitTestData, 
    int result;
    pcchar description;
);
DEFINE_STRUCT(UnitTestTest, 
    List datas;
    pcchar testname;
    pcchar groupname;
    pcchar description;
);

DEFINE_FUNCTION_TYPE(UnitTestCallback, void, (UnitTestTest*));

#define UNITTEST(GROUPNAME, TESTNAME, DESCRIPTION, BODY) \
    void UnitTestCallback_##GROUPNAME##_##TESTNAME(UnitTestData* test){ BODY }


