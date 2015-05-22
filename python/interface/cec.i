
%module cec
%{
#define SWIG_FILE_WITH_INIT
#include "cec_configuration.hpp"
%}

class CecConfiguration {
public:
    CecConfiguration();
    ~CecConfiguration();
};

