#include <sstream>
#include "log.h"
#include "svm_basic.h"

void Log::log(int level, std::string text) {
	if (level <= VERBOSITY)
		std::cout << text << std::endl << std::flush;
}
