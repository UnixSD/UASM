%define EXAMPLE_H
%ifdef EXAMPLE_H
%define EXAMPLE_TRUE
%undef DEF_1
%undef DEF_2
%undef DEF_DEF_DEF
%undef FED
%undef DEFDEFDEFDEFDEFDEF
%include <stdio.h>
%include <iostream>
%define DEFDEFDEF mov eax, ebx
%define DEFINE    8237465128
%define __DEFINE__ "ok"
%define _DEFINE_   "okok"
%endif

.start:
    DEFDEFDEF
    DEFINE
    __DEFINE__
    _DEFINE_