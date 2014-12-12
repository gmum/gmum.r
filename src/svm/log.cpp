#include <sstream>
#include "log.h"

Logger::Logger() {
  verbosity = LogLevel::Info;
}

void Logger::log(int level, std::string text) {
	if (level <= verbosity)
		std::cout << text << std::endl << std::flush;
}