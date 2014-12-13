#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>
#include <RcppArmadillo.h>
#include <R.h>
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

	int degree;		// for poly
	double gamma;	// for poly/rbf/sigmoid
	double coef0;	// for poly/sigmoid

	//these are for training only
	double cache_size; 	// in MB
	double eps;			// stopping criteria
	double C;			// for C_SVC, EPSILON_SVR and NU_SVR
	int nr_weight;		// for C_SVC
	int *weight_label;	// for C_SVC
	double* weight;		// for C_SVC
	int shrinking;		// use the shrinking heuristics
	int probability; 	// do probability estimates
	
	//	libsvm Model parameters
	int l;
	int nr_class; /* number of classes, = 2 in regression/one class svm */
	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	struct svm_node **SV; /* SVs (SV[l]) */
	double **sv_coef; /* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho; /* constants in decision functions (rho[k*(k-1)/2]) */
	int *sv_indices; /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

	/* for classification only */

	int *label; /* label of each class (label[k]) */
	int *nSV; /* number of SVs for each class (nSV[k]) */
	/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */


	/*TODO: neccessery? check what are they doing */
	double nu; /* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p; /* for EPSILON_SVR */

	arma::mat data;		// armadillo matrix and vector (double)
	arma::vec target;
	arma::vec result;


	arma::mat tmp_data;
	arma::mat tmp_target;
	
	Logger log;

	//universal parameters
	arma::vec w; //d
	double pos_target;
	double neg_target;
	arma::mat arma_SV;

	//2eParameters
	double cov_eps_smoothing;
	arma::mat inv_of_sqrt_of_cov;

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

	void setWeights( Rcpp::NumericVector );
	void setLibrary( std::string );
	void setKernel( std::string );
	void setPreprocess( std::string );
  double getB();
  // logger
  void set_verbosity( int );
};

#endif
