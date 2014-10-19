#ifndef GMUM_R_UTILS_H
#define GMUM_R_UTILS_H

namespace gmum {

    /// Execute a command from shell and get its stdout as std::string
    std::string execute_command( std::string command );

    /// Split string into a vector of lines
    std::vector< std::string > split_lines( std::string given_string );
};

#endif
