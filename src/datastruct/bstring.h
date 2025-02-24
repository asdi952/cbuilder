#pragma once
#include <pch.h>


DEFINE_STRUCT(Bstring,
    pcchar ptr;
    uint length;
);

Bstring bstring_from_pcchar( pcchar pch);
Bstring bstring_from_pcchar_s( pcchar pch, uint len);
