/**
 * @file        svmlight_runner.h
 * @brief       SVMLight implementation class of SVMHandler -- headers
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   MIT
 */

#ifndef SVMLIGHTRUNNER_H
#define SVMLIGHTRUNNER_H

#include "svm_handler.h"
#include "svm_basic.h"

#define SVMLightRunner_temp_model_file "svmlightrunner_temp_model_file"
#define SVMLightRunner_temp_output_file "svmlightrunner_temp_output_file"

/// SVMLight library implementation of SVMHandler
class SVMLightRunner : public SVMHandler {

public :

    /// Default constructor
    SVMLightRunner();
    /// Default destructor
    ~SVMLightRunner();
    // Documented in the parent class
    SVMResult processRequest( SVMConfiguration, SVMResult );
    // Documented in the parent class
    bool canHandle( SVMConfiguration );
    /// Train, test and store results
    std::string processSVMLightFiles(
        std::string training_file,
        std::string test_file
    );

protected :

    /// SVMLight's command line `svm_learn` equivalent
    std::string librarySVMLearn( std::string arguments );
    /// SVMLight's ommand line `svm_classify` equivalent
    std::string librarySVMClassify( std::string arguments );
};

#endif
