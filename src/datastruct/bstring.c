#include <pch.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "bstring.h"

Bstring bstring_from_pcchar( pcchar pch){
    uint len = strlen(pch);
    void* pt = malloc(sizeof(char) * len);
    memcpy(pt, pch, len);
    Bstring str = {.ptr = pt, .length = len};
    return str;
}

Bstring bstring_from_pcchar_s( pcchar pch, uint len){
    void* pt = malloc(sizeof(char) * len);
    memcpy(pt, pch, len);
    Bstring str = {.ptr = pt, .length = len};
    return str;
}