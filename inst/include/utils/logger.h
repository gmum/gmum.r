#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include "threading.h"

#ifdef RCPP_INTERFACE
    #include <RcppArmadillo.h>
    // TODO: remove it
    using namespace Rcpp;
    #define COUT(x) Rcpp::Rcout<<(x)<<std::endl<<std::flush;
    #define CERR(x) Rcpp::Rcerr<<(x)<<std::endl<<std::flush;
#else
    #include <armadillo>
    // TODO: remove it
    using namespace arma;
    #define COUT(x) std::cout<<(x)<<std::endl<<std::flush;
    #define CERR(x) std::cerr<<(x)<<std::endl<<std::flush;
#endif

#define LOG(logger, level, text) logger.log(level, text);
  #define LOG_PTR(logger, level, text) logger->log(level, text);
  #ifdef DEBUG_GMUM
    #define DBG(logger, level, text) logger.log(level, text);
    #define DBG_PTR(logger, level, text) logger->log(level, text);
  #else
    #define DBG(logger, level, text)
    #define DBG_PTR(logger, level, text)
#endif


class LogLevel {
public:
    static const int NO_LOGGING_LEVEL = 0;
    static const int FATAL_LEVEL = 1;
    static const int ERR_LEVEL = 2;
    static const int WARNING_LEVEL = 3;
    static const int INFO_LEVEL = 4;
    static const int DEBUG_LEVEL = 5;
    static const int TRACE_LEVEL = 6;
};

class Logger {
public:
    std::ostream * out; // Pointer to support copy/assignment constructor
    int verbosity;
    // This mutex exists only because we cannot print to cout in R in separate thread
    // And there will be master thread printing logs
    gmum::fast_mutex mutex;

    // TODO: It is bad design to add that many macros. We should separate this logic during construction
#ifdef RCPP_INTERFACE
    Logger(int level=LogLevel::INFO_LEVEL,
           std::ostream & out = Rcpp::Rcout): out(&out), verbosity(level) {
    }
#else
    Logger(int level=LogLevel::INFO_LEVEL,
           std::ostream & out = std::cout): out(&out), verbosity(level) {
    }
#endif

    template<class T>
    void log(int level, T msg) {
        if (level <= verbosity) {
            gmum::scoped_lock<gmum::fast_mutex> scoped_lock(mutex);
            (*out)<<msg<<std::endl;
        }
    }
};

#endif /* LOGGER_H */
