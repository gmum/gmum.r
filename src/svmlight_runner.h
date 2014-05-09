/// svmlight_runner.h

#ifndef SVMLIGHTRUNNER_H
#define SVMLIGHTRUNNER_H

#include "svm_handler.h"
#include "svm_basic.h"

#define SVMLightRunner_temp_model_file "svmlightrunner_temp_model_file"
#define SVMLightRunner_temp_output_file "svmlightrunner_temp_output_file"

class SVMLightRunner : public SVMHandler {

public :

    SVMLightRunner();
    ~SVMLightRunner();

    SVMResult processRequest( SVMConfiguration, SVMResult );

    bool canHandle( SVMConfiguration );

    /// Train, test and store results
    std::string processSVMLightFiles(
        std::string training_file,
        std::string test_file
    );

protected :

    /// `svm_learn` wrapper
    std::string librarySVMLearn( std::string arguments );
    /// `svm_classify` wrapper
    std::string librarySVMClassify( std::string arguments );
};

/// Temporary exemplary method available from within R workspace
RcppExport SEXP svmlight_process_files( SEXP, SEXP );

#endif
