#include <ctype.h>
#include <cstring>

#include "utils/utils.h"

int check_argc(const char* input) {
    int argc = 0;
    int len = strlen(input);

    bool reading_arg = false;
    char c;
    for (int i = 0; i < len; ++i) {
        c = input[i];
        if (isspace(c)) {
            reading_arg = false;
        } else if (not reading_arg) {
            reading_arg = true;
            ++argc;
        }
    }
    return argc;
}

int check_argc(const std::string input) {
    char* cstr = new char [input.length() + 1];
    std::strcpy(cstr, input.c_str());

    int argc = check_argc(cstr);

    delete[] cstr;
    return argc;
}

char** to_argv(const char* input) {
    int argc = check_argc(input);

    char** argv;
    char* arg;
    char* in_copy;

    if ((argv = (char**)malloc(argc * sizeof(char*))) == NULL) {
        throw BasicException("to_argv(): argv memory allocation error");
    }

    in_copy = (char*)malloc(strlen(input) + 1);
    strcpy(in_copy, input);
    for (int i = 0; i < argc; ++i) {
        // Whitespaces handled by isspace() function
        arg = strtok(in_copy, " \t\n\v\f\r");
        in_copy = NULL;
        if (arg == NULL) {
            throw BasicException("to_argv(): argc is not equal to real count");
        }
        // NOTE: sizeof(char) is always 1
        if ((argv[i] = (char*)malloc(strlen(arg) + 1)) == NULL) {
            throw BasicException("to_argv(): argv[i] memory allocation error");
        }
        strcpy(argv[i], arg);
    }
    return argv;
}

char** to_argv(const std::string input) {
    char* cstr = new char [input.length() + 1];
    std::strcpy(cstr, input.c_str());

    char** argv = to_argv(cstr);

    delete[] cstr;
    return argv;
}

char** free_argv(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
    return NULL;
}

#ifdef RCPP_INTERFACE
int rcpp_c_rand() {
    return int(Rcpp::runif(1)[0] * INT_MAX);
}
#endif

int ed_c_rand() {
#ifdef RCPP_INTERFACE
    return rcpp_c_rand();
#else
    return rand();
#endif
}

void ed_c_srand(unsigned int seed) {
#ifdef RCPP_INTERFACE
    Rcpp::Environment::global_env()[".Random.seed"] = seed;
#else
    srand(seed);
#endif
}
