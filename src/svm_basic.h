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

	std::string filename;
	std::string model_filename;
	std::string output_filename;
	bool prediction;

public:
	SVMType svm_type;
	//KernelType kernel_type;
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
