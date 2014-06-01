#include "svm_basic.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
	this->prediction = false;
	SVMConfiguration::setDefaultParams();
}
SVMConfiguration::SVMConfiguration(bool prediction) {
	this->prediction = prediction;
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

void SVMConfiguration::setData(arma::mat data) {
	this->data = data;
}

arma::mat SVMConfiguration::getData() {
	return this->data;
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

void SVMConfiguration::createParams(std::string kernel_type,
		std::string svm_type, std::string preprocess, int degree, double gamma,
		double coef0) {
	if (preprocess == "norm") {
		Preprocess prep = NORM;
		this->preprocess = prep;
	} else {
		Preprocess prep = NONE;
		this->preprocess = prep;
	}
}

void SVMConfiguration::setWeights( Rcpp::NumericVector weights ) {
	this->nr_weight = weights.size();
	this->weight = new double[nr_weight];

	for (int i = 0; i < nr_weight; i++) {
		weight[i] = weights(1);
	}
}

void SVMConfiguration::setLibrary( std::string library ) {
	if ( library == "libsvm" ) {
		this->svm_type = LIBSVM;
	}
	// else if
}

void SVMConfiguration::setKernel( std::string kernel ) {
	if ( kernel == "linear" ) {
		this->kernel_type = _LINEAR;
	}
	else if ( kernel == "poly" ) {
		this->kernel_type = _POLY;
	}
	else if ( kernel == "rbf" ) {
		this->kernel_type = _RBF;
	}
	else if ( kernel == "sigmoid" ) {
		this->kernel_type = _SIGMOID;
	}
}

void SVMConfiguration::setPreprocess( std::string preprocess ) {
	if ( preprocess ==  "2e" ) {
		this->preprocess = TWOE;
	}
	else if ( preprocess == "none" ) {
		this->prediction = NONE;
	}
}

void SVMConfiguration::setDefaultParams() {
	svm_type = LIBSVM;
	kernel_type = _RBF;
	degree = 3;
	gamma = 0;	// 1/num_features
	coef0 = 0;
	cache_size = 100;
	C = 1;
	eps = 1e-3;
	shrinking = 1;
	probability = 0;
	nr_weight = 0;
	weight_label = NULL;
	weight = NULL;
}

