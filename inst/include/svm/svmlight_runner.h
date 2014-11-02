/**
 * @file        svmlight_runner.h
 * @brief       SVMLight implementation class of SVMHandler headers
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   GPLv3
 */

#ifndef SVMLIGHT_RUNNER_H
#define SVMLIGHT_RUNNER_H

#include "svm_handler.h"


extern "C" {
#include "inst/include/svmlight/svm_common.h"
#include "inst/include/svmlight/svm_learn.h"
}
#define SVMLightRunner_temp_model_file "svmlightrunner_temp_model_file"
#define SVMLightRunner_temp_output_file "svmlightrunner_temp_output_file"

/// SVMLight library implementation of SVMHandler
class SVMLightRunner: public SVMHandler {

public:

    /// Default constructor
    SVMLightRunner();

    /// Default destructor
    ~SVMLightRunner();

    // Documented in the parent class
    bool canHandle(SVMConfiguration &);

    // Documented in the parent class
    void processRequest(SVMConfiguration &);

//protected:

    /** @name Library functionalities wrappers
     *  Following methods are direct library functionalities wrappers with
     *  file operations parametrized.
     */
    /// @{

    /**
     * SVMLight's `svm_learn` main method
     * @author  Thorsten Joachims
     */
    int librarySVMLearnMain(int argc, char **argv, bool use_gmumr,
        SVMConfiguration &config
    );

    /**
     * SVMLight's `svm_learn` auxiliary method
     * @author  Thorsten Joachims
     */
    void libraryReadInputParameters(
        int argc, char *argv[], char *docfile, char *modelfile,
        char *restartfile, long *verbosity, LEARN_PARM *learn_parm,
        KERNEL_PARM *kernel_parm, bool use_gmumr, SVMConfiguration &config
    );

    /**
     * SVMLight's auxiliary method
     * @author  Thorsten Joachims
     */
    void libraryReadDocuments(
        char *docfile, DOC ***docs, double **label, long int *totwords,
        long int *totdoc, bool use_gmumr, SVMConfiguration &config
    );

    /// @}

    /**
     * Convert SVMConfiguration to one line of SVMLight input
     */
    char * svmConfigurationToSVMLightInputLine(
        SVMConfiguration &config, long int line_num
    );
};

/* SVMLIGHT_RUNNER_H */
#endif
