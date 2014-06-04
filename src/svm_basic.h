#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>
#include <RcppArmadillo.h>
#include <R.h>

//enum KernelType {
//	LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED
//};

enum SVMType {
	LIBSVM, SVMLIGHT
};

enum Preprocess {
	TWOE, VK, NONE, NORM
};
// NORM is solely for test purposes

// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration {
private:

	std::string filename; //filename with data
	std::string model_filename;
	std::string output_filename;
	bool prediction;

	std::string error_msg; //if something went wrong, there is msg with error

public:

//	Params

	//this is SVM ADDITIONAL type!!!!!!!
	SVMType our_svm_type;
	//Kernel type in svm format
	int kernel_type;
	Preprocess preprocess;

	int svm_type; //enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
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

	/*TODO: Not neccessery? */
	double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
	double p;	/* for EPSILON_SVR */

//	End of params

//	Model parameters
	double **sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho;		/* constants in decision functions (rho[k*(k-1)/2]) */

//
	arma::mat data;		// armadillo matrix and vector (double)
	arma::vec target;
	arma::vec result;

	// constructors
	SVMConfiguration();
	SVMConfiguration(bool);

	// methods
	arma::mat getData();
	void setData(arma::mat);

	void createParams(std::string, std::string, std::string, int, double,
			double);
	void setDefaultParams();

	void setFilename(std::string);
	std::string getFilename();

	void setModelFilename(std::string);
	std::string getModelFilename();

	void setOutputFilename(std::string);
	std::string getOutputFilename();

	void setPrediction(bool);
	bool isPrediction();

};

#endif
