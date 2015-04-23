#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

#include "log.h"

enum KernelType {
    _LINEAR, _POLY, _RBF, _SIGMOID // _PRECOMPUTED
};


enum SVMType {
    LIBSVM, SVMLIGHT
};

enum Preprocess {
    TWOE, NONE
};
// NORM is solely for test purposes

// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration {

public:

    std::string filename; //filename with data
    std::string model_filename;
    std::string output_filename;
    bool prediction;

    std::string error_msg; //if something went wrong, there is msg with error

    SVMType library;
    int svm_type;
    KernelType kernel_type;
    Preprocess preprocess;

    int degree;     // for poly
    double gamma;   // for poly/rbf/sigmoid
    double coef0;   // for poly/sigmoid

    //these are for training only
    double cache_size;  // in MB
    double eps;         // stopping criteria
    double C;           // for C_SVC, EPSILON_SVR and NU_SVR
    double* libsvm_class_weights;     // for C_SVC
    //which weight is for which class
    int* libsvm_class_weights_labels;     // for C_SVC
    int class_weight_length;      // for C_SVC
    int shrinking;      // use the shrinking heuristics
    int probability;    // do probability estimates

    int nr_class; /* number of classes, = 2 in regression/one class svm */

    //libsvm model parameters
    //TODO: delete those variables
    int *label; /* label of each class (label[k]) */
    int *nSV; /* number of SVs for each class (nSV[k]) */
    /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */

    /*TODO: neccessery? check what are they doing */
    double nu; /* for NU_SVC, ONE_CLASS, and NU_SVR */
    double p; /* for EPSILON_SVR */
    int l; //TODO: remove it in svm_ligth

    /* SVMLight parameters */
    char *kernel_parm_custom;   // Custom kernel parameter(s)

    /* Global "stuff" */
    arma::vec alpha_y;          // SVMLight's alpha*y values for SV's
    arma::sp_mat support_vectors;  // sacherus: number of support vectors x data_dim
    arma::mat data;     // armadillo matrix and vector (double)
    arma::vec target;
    arma::vec result;

    // Sparse data
    bool sparse;
    arma::sp_mat sparse_data;

    // Class weights
    arma::vec class_weights;
    bool use_class_weights;

    // Example weights (used by svmlight)
    bool use_example_weights;
    arma::vec example_weights;

    Logger log;

    arma::vec w; //d
    double b;
    double pos_target;
    double neg_target;

    //2eParameters & Variables
    double cov_eps_smoothing_start;
    double cov_eps_smoothing_end;
    arma::mat inv_of_sqrt_of_cov;
    arma::mat tmp_data;
    arma::mat tmp_target;

    // debug
    bool debug_library_predict;


    // constructors
    SVMConfiguration();
    SVMConfiguration(bool);

    // methods
    arma::mat getData();
    void setData(arma::mat);
    void setDefaultParams();

    void setFilename(std::string);
    std::string getFilename();

    void setModelFilename(std::string);
    std::string getModelFilename();

    void setOutputFilename(std::string);
    std::string getOutputFilename();

    void setPrediction(bool);
    bool isPrediction();


    void setLibrary( std::string );
    void setKernel( std::string );
    void setPreprocess( std::string );
    double getB();
    void setB(double b);
    // logger
    void set_verbosity( int );

    void setSparse(bool sparse);

    /**
     * Sets sparse data from CSC sparse matrix format
     */
    void setSparseData(
        arma::uvec rowind,
        arma::uvec colptr,
        arma::vec values,
        size_t n_rows,
        size_t n_cols,
        bool one_indexed=false
    );

    //@param class_weights_labels - needed for libsvm
    void setClassWeights(arma::vec, arma::vec);

    arma::sp_mat &getSparseData();

    bool isSparse();
    int getDataDim();
    int getDataExamplesNumber();
    size_t getSVCount();
};

#endif
