
%module svm
%{
#define SWIG_FILE_WITH_INIT
#include "svm_basic.h"
%}

class SVMConfiguration {
public:
    SVMConfiguration();
    ~SVMConfiguration();
};

