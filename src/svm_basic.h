#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>
#include <RcppArmadillo.h>

//enum KernelType { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; // kernel type
enum {
	LIBSVM, SVMLIGHT
};
// svm type

// This struct will conatin all the necessery svm parameters and will be used in SVMConfig
struct SVMParameters {
	int svm_type;
	int kernel_type;
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
};

struct SVMData {
	arma::mat data;		// armadillo matrix and vector (double)	
	arma::vec target;
	int len;
};

// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration {
private:
	SVMData *data;
	SVMParameters params;
	std::string filename;
	std::string model_filename;
	std::string output_filename;
	bool prediction;

public:
	SVMConfiguration();
	SVMConfiguration(SVMData*, SVMParameters);

	void setData(SEXP, SEXP);
	SVMData* getData();

	void setParams(SVMParameters);
	SVMParameters getParams();

	void setFilename(std::string);
	std::string getFilename();

	void setModelFilename(std::string);
	std::string getModelFilename();

	void setOutputFilename(std::string);
	std::string getOutputFilename();

	void setPrediction(bool);
	bool isPrediction();
};

// Our "output" class containing classification result
class SVMResult {
private:
	SVMData *data;
	std::string message;

public:
	SVMResult();
	SVMResult(std::string);
	SVMResult(SVMData*);
	SVMResult(SVMData*, std::string);

	void setResult(SVMData*);
	SVMData* getResult();

	void setMessage(std::string);
	std::string getMessage();

};

#endif
