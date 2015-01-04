/**
 * @file        svmlight_runner.h
 * @brief       SVMLight implementation class of SVMHandler headers
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   GPLv3
 */

#ifndef SVMLIGHT_RUNNER_H
#define SVMLIGHT_RUNNER_H

#include <string>

#include "svm_handler.h"


extern "C" {
#include "svmlight/svm_common.h"
#include "svmlight/svm_learn.h"
}

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

protected:
    
    // TODO: Move this method to SVMHandler?
    void predict(SVMConfiguration &);

    /** @name Library functionalities wrappers
     *  Following methods are direct library functionalities wrappers with
     *  file operations parametrized (`bool use_gmumr`).
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
    void librarySVMLearnReadInputParameters(
        int argc, char *argv[], char *docfile, char *modelfile,
        char *restartfile, long *verbosity, LEARN_PARM *learn_parm,
        KERNEL_PARM *kernel_parm, bool use_gmumr, SVMConfiguration &config
    );

    /**
     * SVMLight's `svm_classify` main method
     * @author  Thorsten Joachims
     */
    int librarySVMClassifyMain(int argc, char **argv, bool use_gmumr,
        SVMConfiguration &config
    );

    /**
     * SVMLight's `svm_classify` auxiliary method
     * @author  Thorsten Joachims
     */
    void librarySVMClassifyReadInputParameters(
        int argc, char **argv, char *docfile, char *modelfile,
        char *predictionsfile, long int *verbosity, long int *pred_format,
        bool use_gmumr, SVMConfiguration &config
    );

    /**
     * SVMLight's auxiliary method
     * @author  Thorsten Joachims
     */
    MODEL * libraryReadModel(
        char *modelfile, bool use_gmumr, SVMConfiguration &config
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
     * Convert SVMConfiguration to one line of SVMLight's `svm_learn` input
     */
    std::string SVMConfigurationToSVMLightLearnInputLine(
        SVMConfiguration &config, long int line_num
    );

    /**
     * Convert SVMConfiguration to one line of SVMLight's `svm_classify` input
     * of every Support Vector in one line (starting with alpha*y)
     */
    char * SVMConfigurationToSVMLightModelSVLine(
        SVMConfiguration &config, long int line_num
    );

    /**
     * Convert SVMConfiguration to SVMLight's model file lines
     */
    char ** SVMConfigurationToSVMLightModelFile(SVMConfiguration &config);

    /**
     * Store SVMLight model data into SVMConfiguration
     * NOTE: It does not clean unnecessary data!
     */
    void SVMLightModelToSVMConfiguration(
        MODEL *model, SVMConfiguration &config
    );
};

/* SVMLIGHT_RUNNER_H */
#endif

