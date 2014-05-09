#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

string gmum::execute_command( string command ) {
    char* cmd = (char*) command.c_str();
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        return "ERROR: Error opening stream...";
    }
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
            // TODO: Handle `bad_alloc` exception
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

vector< string > gmum::split_lines( string given_string ) {
    istringstream input_string_stream( given_string );
    vector< string > lines;
    copy(
        istream_iterator< string >( input_string_stream ),
        istream_iterator< string >(),
        back_inserter< vector< string > >( lines )
    );
    return lines;
}
