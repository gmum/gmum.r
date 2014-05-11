#include "svm_basic.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
}
SVMConfiguration::SVMConfiguration(SVMData *data, SVMParameters params) {
	this->data = data;
	this->params = params;
}
// Getters and Setters
void SVMConfiguration::setData(SEXP x, SEXP y) {
	Rcpp::NumericMatrix xr(x); // Rccp matrix from R data
	Rcpp::NumericVector yr(y); // Rcpp vector from R data

	arma::mat X(xr.begin(), xr.nrow(), xr.ncol(), false); // reusing memory
	arma::vec Y(yr.begin(), yr.size(), false);

	SVMData data_struct;
	data_struct.data = X;
	data_struct.target = Y;
	data_struct.len = Y.n_elem;

	this->data = &data_struct;
}
SVMData* SVMConfiguration::getData() {
	return this->data;
}
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

// SVM Result
// Constructors
SVMResult::SVMResult() {
}
SVMResult::SVMResult(std::string message) {
	this->message = message;
}
SVMResult::SVMResult(SVMData *data) {
	this->data = data;
}
// Getters and Setters
void SVMResult::setData(SVMData *data) {
	this->data = data;
}
SVMData* SVMResult::getData() {
	return this->data;
}
void SVMResult::setMessage(std::string message) {
	this->message = message;
}
std::string SVMResult::getMessage() {
	return this->message;
}

