#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger {
	int m_verbosity;
public:
	Logger(int verbosity): m_verbosity(verbosity){

	}
    void log(int level, std::string line){
                if(level>= m_verbosity) std::cerr<<line<<std::endl<<std::flush;
    }
};

#endif

