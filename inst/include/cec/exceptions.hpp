#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

#ifdef SWIG
%{
#define SWIG_FILE_WITH_INIT
#include "exceptions.hpp"
%}
#endif

class NoPointsInCluster: public std::exception {
};

#endif
