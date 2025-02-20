#pragma once
#include "defines.h"
#include <pch.h>

DEFINE_STRUCT(UnitTestData, 
);

DEFINE_FUNCTION_TYPE(UnitTestCallback, void, (UnitTestData*));

#define UNITTEST(GROUPNAME, TESTNAME, DESCRIPTION, BODY) \
    void UnitTestCallbac_##GROUPNAME##_##TESTNAME(UnitTestData* data){ BODY}


