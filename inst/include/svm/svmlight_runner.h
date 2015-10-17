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
    void processRequest(
        SVMConfiguration &
    );

protected:
    
    /**
     * Store numeric results as user-defined labels
     */
    void resultsToLabels(SVMConfiguration &);

    /**
     * Clear globals that are assuming we just started command-line svmlight
     */
    void _clear();

    /** @name Library functionalities wrappers
     *  Following methods and fields are direct library functionalities
     *  wrappers with file operations parametrized (`bool use_gmumr`).
     */
    /// @{

    /* These strings are not used by us (we are operating only on memory)
     * but let's keep them before removing unnecessary code */
    char docfile[200];           /* file with training examples */
    char modelfile[200];         /* file for resulting classifier */
    char restartfile[200];       /* file with initial alphas */
    char predictionsfile[200];

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

    /**
     * SVMLight's auxiliary method
     * @author  Thorsten Joachims
     */
    void libraryWaitAnyKey();

    /**
     * SVMLight's auxiliary method
     * @author  Thorsten Joachims
     */
    void libraryPrintHelp();

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
     * Store SVMLight model data into SVMConfiguration
     * NOTE: It does not clean unnecessary data!
     */
    void SVMLightModelToSVMConfiguration(
        MODEL *model, SVMConfiguration &config
    );
};

/* SVMLIGHT_RUNNER_H */
#endif

