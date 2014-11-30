#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

#ifdef DEBUG
#define LOG(logger, level, text) logger.log(level, text);
#define DBG(logger, level, text) logger.log(level, text);
#else
#define LOG(logger, level, text) logger.log(level, text);
#define DBG(logger, level, text)
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

class Logger {
public:

	int verbosity;

	Logger();

	void log(int level, std::string text);

};

#endif /* LOG_H */
