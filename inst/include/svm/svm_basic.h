#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>
#include <RcppArmadillo.h>
#include <R.h>
#include "log.h"

enum KernelType {
	kLinear, kPoly, kRBF, kSigmoid // kPrecomputed
};


enum SVMType {
	kLibSMV, kSVMLight
};

enum Preprocess {
	kTwoE, kNone
};
// NORM is solely for test purposes

// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration {

public:

	std::string filename_; //filename with data
	std::string model_filename_;
	std::string output_filename_;
	bool prediction_;

	std::string error_msg_; //if something went wrong, there is msg with error

	SVMType library_;
	int svm_type_;
	KernelType kernel_type_;
	Preprocess preprocess_;

	int degree_;		// for poly
	double gamma_;	// for poly/rbf/sigmoid
	double coef0_;	// for poly/sigmoid

	//these are for training only
	double cache_size_; 	// in MB
	double eps_;			// stopping criteria
	double c_;			// for C_SVC, EPSILON_SVR and NU_SVR
	int nr_weight_;		// for C_SVC
	int *weight_label_;	// for C_SVC
	double* weight_;		// for C_SVC
	int shrinking_;		// use the shrinking heuristics
	int probability_; 	// do probability estimates
	
	//	libsvm Model parameters
	int l_;
	int nr_class_; /* number of classes, = 2 in regression/one class svm */
	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	struct svm_node **sv_; /* SVs (SV[l]) */
	double **sv_coef_; /* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho_; /* constants in decision functions (rho[k*(k-1)/2]) */
	int *sv_indices_; /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

    /* SVMLight parameters */
    double threshold_b_;
    char *kernel_parm_custom_;   // Custom kernel parameter(s)
    arma::vec alpha_y_;          // SVMLight's alpha*y values for SV's
    arma::mat support_vectors_;
    // User-defined classification mode labels
    int label_negative_;
    int label_positive_;


	/* for classification only */

	int *label_; /* label of each class (label[k]) */
	int *n_sv_; /* number of SVs for each class (nSV[k]) */
	/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */


	/*TODO: neccessery? check what are they doing */
	double nu_; /* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p_; /* for EPSILON_SVR */

	arma::mat data_;		// armadillo matrix and vector (double)
	arma::vec target_;
	arma::vec result_;


	arma::mat tmp_data_;
	arma::mat tmp_target_;
	
	Logger log_;

	//universal parameters
	arma::vec w_; //d
	double pos_target_;
	double neg_target_;
	arma::mat arma_sv_;

	//2eParameters
	double cov_eps_smoothing_;
	arma::mat inv_of_sqrt_of_cov_;

	// constructors
	SVMConfiguration();
	SVMConfiguration(bool);

	// methods
	arma::mat data();
	void set_data(arma::mat);
	void set_default_params();

	void set_filename(std::string);
	std::string filename();

	void set_model_filename(std::string);
	std::string model_filename();

	void set_output_filename(std::string);
	std::string output_filename();

	void set_prediction(bool);
	bool prediction();

	void set_weights( Rcpp::NumericVector );
	void set_library( std::string );
	void set_kernel( std::string );
	void set_preprocess( std::string );
    double b();
    // logger
    void set_verbosity( int );
};

#endif
