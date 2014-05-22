#include "svm_basic.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
	this->prediction = true;
}
SVMConfiguration::SVMConfiguration( SVMParameters params, bool prediction ) {
	this->params = params;
	this->prediction = prediction;
}
// Getters and Setters
//void SVMConfiguration::setData(SEXP x, SEXP y) {
//	Rcpp::NumericMatrix xr(x); // Rccp matrix from R data
//	Rcpp::NumericVector yr(y); // Rcpp vector from R data
//
//	arma::mat X(xr.begin(), xr.nrow(), xr.ncol(), false); // reusing memory
//	arma::vec Y(yr.begin(), yr.size(), false);
//
//	SVMData data_struct;
//	data_struct.data = X;
//	data_struct.target = Y;
//	data_struct.len = Y.n_elem;
//
//	this->data = &data_struct;
//}

void SVMConfiguration::setParams(SVMParameters params) {
	this->params = params;
}
SVMParameters SVMConfiguration::getParams() {
	return this->params;
}

void SVMConfiguration::setFilename(std::string filename) {
	this->filename = filename;
}
std::string SVMConfiguration::getFilename() {
	return this->filename;
}

void SVMConfiguration::setModelFilename(std::string filename) {
	this->model_filename = filename;
}
std::string SVMConfiguration::getModelFilename() {
	return this->model_filename;
}

void SVMConfiguration::setOutputFilename(std::string filename) {
	this->output_filename = filename;
}
std::string SVMConfiguration::getOutputFilename() {
	return this->output_filename;
}

bool SVMConfiguration::isPrediction() {
	return this->prediction;
}

void SVMConfiguration::setPrediction(bool prediction) {
	this->prediction = prediction;
}

void SVMConfiguration::createParams(
		std::string kernel_type,
		std::string svm_type,
		std::string preprocess,
		int degree,
		double gamma,
		double coef0
){
	SVMParameters params;
	if ( preprocess == "norm" ) {
		Preprocess prep = NORM;
		params.preprocess = prep;
	}
	else {
		Preprocess prep = NONE;
		params.preprocess = prep;
	}


	this->setParams(params);

}

