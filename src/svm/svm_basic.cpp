#include "svm_basic.h"
#include "svm.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
	this->prediction = false;
	SVMConfiguration::setDefaultParams();
}


int SVMConfiguration::getDataDim() {
	if(isSparse()) {
		return this->data_dim;
	} else {
		return this->data.n_cols;
	}
}

SVMConfiguration::SVMConfiguration(bool prediction) {
	this->prediction = prediction;
}

void SVMConfiguration::setSparse(bool sparse) {
	this->sparse = sparse;
}

bool SVMConfiguration::isSparse() {
	return this->sparse;
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

void SVMConfiguration::setWeights( Rcpp::NumericVector weights ) {
	this->nr_weight = weights.size();
	this->weight = new double[nr_weight];

	for (int i = 0; i < nr_weight; i++) {
		weight[i] = weights(1);
	}
}

void SVMConfiguration::setLibrary( std::string library ) {
	if ( library == "libsvm" ) {
		this->library = LIBSVM;
		this->svm_type = C_SVC;
	}
	else if (library == "svmlight" ) {
    this->library = SVMLIGHT;
    this->svm_type = C_SVC;
	}
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
		this->preprocess = NONE;
	}
}

void SVMConfiguration::set_verbosity(int verbosity){
  this->log.verbosity = verbosity;
}

double SVMConfiguration::getB() {
  return threshold_b;
}

void SVMConfiguration::setB(double b) {
  threshold_b = b;
}

void SVMConfiguration::setDefaultParams() {
	library = LIBSVM;
	svm_type = C_SVC;
	kernel_type = _LINEAR;
  preprocess = NONE;
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
	cov_eps_smoothing = 1.0e-10;
//	Probably not necessery
	nu = 0.5;
	p = 0.1;

    // User-defined classification mode labels
    // (will be filled during data processing)
    neg_target = 0;
    pos_target = 0;
}

