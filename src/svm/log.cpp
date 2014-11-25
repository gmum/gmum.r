#include <sstream>
#include "log.h"

Log::Log() {
	verbosity = LogLevel::Info;
}

void Log::log(int level, std::string text) {
	if (level <= verbosity)
		std::cout << text << std::endl << std::flush;
}
