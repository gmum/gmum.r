#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
using namespace Rcpp;
#define COUT(x) Rcpp::Rcout<<(x)<<std::endl<<std::flush;
#define CERR(x) Rcpp::Rcerr<<(x)<<std::endl<<std::flush;
#else
#define COUT(x) cout<<(x)<<std::endl<<std::flush;
#define CERR(x) cerr<<(x)<<std::endl<<std::flush;
#endif

#define LOG(logger, level, text) logger.log(level, text);
  #define LOG_PTR(logger, level, text) logger->log(level, text);
    #ifdef DEBUG
    #define DBG(logger, level, text) logger.log(level, text);
    #define DBG_PTR(logger, level, text) logger->log(level, text);
  #else
    #define DBG(logger, level, text)
    #define DBG_PTR(logger, level, text)
#endif

class LogLevel {
public:

    static const int NO_LOGGING  = 0;
    static const int FATAL       = 1;
    static const int ERR         = 2;
    static const int WARNING     = 3;
    static const int INFO        = 4;
    static const int DEBUG       = 5;
    static const int TRACE       = 6;

};

class Logger {
public:

    int verbosity;

    Logger() { 
        verbosity = LogLevel::INFO;
    }

	Logger(int level): verbosity(level) {}

    void log(int level, std::string text) {
        if (level <= verbosity) {
            COUT(text);
        }
    }

    void log(int level, int number) {
        if (level <= verbosity) {
            std::stringstream ss;
            ss << number;
            COUT(ss.str());
        }
    }

    void log(int level, double number) {
        if (level <= verbosity) {
            std::stringstream ss;
            ss << number;
            COUT(ss.str());
        }
    }

};

#endif /* LOGGER_H */
