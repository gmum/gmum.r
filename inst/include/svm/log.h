#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <sstream>

#ifdef DEBUG
#define LOG(logger, level, text) logger.Log(level, text);
#define DBG(logger, level, text) logger.Log(level, text);
#else
#define LOG(logger, level, text) logger.Log(level, text);
#define DBG(logger, level, text)
#endif

class LogLevel {
public:

	static const int kNoLogging  = 0;
	static const int kFatal      = 1;
	static const int kError      = 2;
	static const int kWarning    = 3;
	static const int kInfo       = 4;
	static const int kDebug      = 5;
	static const int kTrace      = 6;

};

class Logger {
public:

	int verbosity_;

	Logger() {
		verbosity_ = LogLevel::kInfo;
	}

	void Log(int level, std::string text) {
		if (level <= verbosity_)
			std::cout << text << std::endl << std::flush;
	}

};

#endif /* LOG_H */

