#include "pch.h"
#include "log.h"
#include <list.h>
#include <bstring.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"

List(Errormsg) error_stack;

void error_init(){
    list_init(&error_stack, sizeof(Errormsg));
}

void error_deinit(){
    list_deinit(&error_stack);
}

void errorset_pcchar(pcchar msg){
    Errormsg* emsg = list_emplace_back(&error_stack);
    emsg->type = Errortype_pcchar;
    emsg->pcchar = msg;
}

void errorset_fmt(pcchar fmt, ...){
    Errormsg* emsg = list_emplace_back(&error_stack);
    emsg->type = Errortype_pcchar;
    va_list args;
    va_start(args ,fmt);

    char buff[500];
    int len = sprintf(buff, fmt, args);

    Bstring string = bstring_from_pcchar_s(buff, len);
    emsg->string = string;

    va_end(args);
}
void errorset_bstring(Bstring msg){
    Errormsg* emsg = list_emplace_back(&error_stack);
    emsg->type = Errortype_bstring;
    emsg->string = msg;
}

void term_print_error_stack(){
    if(error_stack.count > 0){
        elog("ERROR STACK:\n");
        FORLIST(&error_stack, i){
            Errormsg* msg = list_get_ptr(&error_stack, i);
            switch(msg->type){
                case Errortype_pcchar:
                    elog("\t%s\n", msg->pcchar);
                break;
                case Errortype_bstring:
                    elog("\t%.*s\n", msg->string.length, msg->string.ptr);
                break;
            }
        }
    }
}