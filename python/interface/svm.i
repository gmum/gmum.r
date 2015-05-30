
%module svm
%{
#define SWIG_FILE_WITH_INIT
#include "svm_basic.h"
%}

enum KernelType {
};

enum SVMType {
    LIBSVM, SVMLIGHT
};

enum Preprocess {
};

class SVMConfiguration {
public:
    SVMConfiguration();
    ~SVMConfiguration();
};

