#pragma once
#include "pch.h"
#include <list.h>
#include <bstring.h>

DEFINE_ENUM(Errortype, 
    Errortype_pcchar,
    Errortype_bstring,
);

DEFINE_STRUCT(Errormsg, 
    Errortype type;
    union{
        Bstring string;
        pcchar pcchar;
    };
);

extern List(Errormsg) error_stack;

void error_init();

void error_deinit();

void errorset_pcchar(pcchar msg);
void errorset_bstring(Bstring msg);
void errorset_fmt(pcchar fmt, ...);


void term_print_error_stack();
