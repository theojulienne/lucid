%module(directors="1") vtest
%feature("director") Foo;
%{
#include "vtest.hpp"
%}

%include "vtest.hpp"
