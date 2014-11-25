#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
//#include "svm_basic.h"

#ifdef DEBUG
#define LOG(logger, level, text) logger.log(level, text);
#define DBG(logger, level, text) logger.log(level, text);
//#define REPORT_PRODUCTION(x) std::cout << (x) << std::endl << std::flush;
#else
#define LOG(logger, level, text) logger.log(level, text);
#define DBG(logger, level, text)
//#define REPORT_PRODUCTION(x) std::cout << (x) << std::endl << std::flush;
#endif

class LogLevel {
public:

	static const int NoLogging  = 0;
	static const int Fatal      = 1;
	static const int Error      = 2;
	static const int Warning    = 3;
	static const int Info       = 4;
	static const int Debug      = 5;
	static const int Trace      = 6;

};

class Log {
public:

	int verbosity;

	Log();

	void log(int level, std::string text);

};

#endif /* LOG_H */
