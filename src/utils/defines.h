#pragma once
#include "types.h"

#define DEFINE_STRUCT(NAME, BODY) typedef struct{ BODY } NAME
#define DEFINE_STRUCT_ANONIMOUS(NAME, BODY) struct{ BODY } NAME
#define DEFINE_ENUM(NAME, ...) typedef enum{ __VA_ARGS__ } NAME

#define DEFINE_FUNCTION_TYPE(NAME, RETURN, PARAMS) typedef RETURN (*NAME) PARAMS
#define DEFINE_FUNCTION(NAME, RETURN, PARAMS) RETURN (*NAME) PARAMS

#define FORCOUNT(COUNT, i) for(uint i = 0; i < COUNT; i++)
#define FORARRAY(NAME, i) for(uint i = 0; i < NAME##_count; i++)

#define TODO(MSG) _Pragma(MSG)

#define UNIQUEID 
    static uint __unique_id = 0;
    static uint getUniqueId(){
        uint holder = __unique_id;
        __unique_id++;
        return holder;
    }