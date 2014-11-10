#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include "svm_basic.h"

#ifdef DEBUG
#define LOG(level, text) Log::log(level, text);
#define REPORT_PRODUCTION(x) std::cout << #x << "=" << (x) << std::endl << std::flush;
#else
#define LOG(level, text)
#define REPORT_PRODUCTION(x) std::cout << #x << "=" << (x) << std::endl << std::flush;
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

	static void log(int level, std::string text);

};

#endif /* LOG_H */
