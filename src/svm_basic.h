#ifndef SVM_BASIC_H
#define SVM_BASIC_H

#include <string>
//#include <Rcpp.h>

//enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; // kernel type
enum { LIBSVM, SVMLIGHT }; // svm type

// This struct will conatin all the necessery svm parameters and will be used in SVMConfig
struct SVM_Parameters {
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

struct SVM_Node {
	int index;
	double value;
};

struct SVM_Data {
	//SEXP r_data;		// raw data from R
	int length;	
	double *target;	
	struct svm_Node **data;
};
 
// Our "input" class containing SVM paramaters and data to be classified
class SVM_Configuration  {
private :
	SVM_Data *data; 
  	SVM_Parameters params;
  	std::string filename;
  	std::string model_filename;
  	std::string output_filename;
  	bool prediction;

public :	
	SVM_Configuration ();
	SVM_Configuration ( SVM_Data*, SVM_Parameters ); 

	void setData( SVM_Data*);
	SVM_Data* getData();

	void setParams( SVM_Parameters );
	SVM_Parameters getParams();

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
class SVM_Result {
private :
	SVM_Data *data; 
  	std::string message;

public : 	
	SVM_Result();
  	SVM_Result( std::string );
	SVM_Result( SVM_Data* ); 
	SVM_Result( SVM_Data*, std::string );

	void setResult( SVM_Data* );
	SVM_Data* getResult();

	void setMessage( std::string );
	std::string getMessage();


};

#endif
