/**
 * @file        svmlight_runner.h
 * @brief       SVMLight implementation class of SVMHandler headers
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   GPLv3
 */

#ifndef SVMLIGHT_RUNNER_H
#define SVMLIGHT_RUNNER_H

#include "svm_handler.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "inst/include/svmlight/svm_common.h"
#include "inst/include/svmlight/svm_learn.h"
#ifdef __cplusplus
}
#endif

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
    void processRequest(SVMConfiguration &);

    // Documented in the parent class
    bool canHandle(SVMConfiguration &);

    /**
     * @brief   Train, test and store results
     * @param   training_file   a file with a training data in
     *                          SVMLight-familiar format
     * @param   test_file       a file with a test data in SVMLight-familiar
     *                          format
     *
     * This method is using training_file and test_file to perform one simple
     * SVMLight task: train and test. Result is an output of SVMLight's
     * `svm_classify` command.
     */
    std::string processSVMLightFiles(
        std::string training_file,
        std::string test_file
    );

    // TODO: Remove this method when useless
    std::vector < std::string > test_svmlight_files();

    /// Temporary exemplary method available from within R workspace
    void test_svmlight();

//protected:

    /** @name Library functionalities wrappers
     *  Below methods are direct library functionalities wrappers. They are
     *  designed to perform the same actions as corresponding command do.
     */
    /// @{

    /**
     * SVMLight's command line `svm_learn` equivalent
     * @param   arguments   command line arguments of `svm_learn` command
     */
    std::string librarySVMLearn();

    /**
     * SVMLight's `svm_learn` main method
     * @author  Thorsten Joachims
     */
    int librarySVMLearnMain(int argc, char *argv[]);

    /**
     * SVMLight's `svm_learn` auxiliary method
     * @author  Thorsten Joachims
     */
    void libraryReadInputParameters(
        int argc, char *argv[], char *docfile, char *modelfile,
        char *restartfile, long *verbosity,
        LEARN_PARM *learn_parm, KERNEL_PARM *kernel_parm);

    /**
     * SVMLight's ommand line `svm_classify` equivalent
     * @param   arguments   command line arguments of `svm_classify` command
     */
    std::string librarySVMClassify( std::string arguments );

    /// @}
};

/* SVMLIGHT_RUNNER_H */
#endif
