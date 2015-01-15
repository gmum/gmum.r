#include "svm_basic.h"
#include "svm.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
	this->prediction_ = false;
	SVMConfiguration::set_default_params();
}

SVMConfiguration::SVMConfiguration(bool prediction) {
	this->prediction_ = prediction;
}

void SVMConfiguration::set_filename(std::string filename) {
	this->filename_ = filename;
}
std::string SVMConfiguration::filename() {
	return this->filename_;
}

void SVMConfiguration::set_model_filename(std::string filename) {
	this->model_filename_ = filename;
}
std::string SVMConfiguration::model_filename() {
	return this->model_filename_;
}

void SVMConfiguration::set_data(arma::mat data) {
	this->data_ = data;
}

arma::mat SVMConfiguration::data() {
	return this->data_;
}

void SVMConfiguration::set_output_filename(std::string filename) {
	this->output_filename_ = filename;
}
std::string SVMConfiguration::output_filename() {
	return this->output_filename_;
}

bool SVMConfiguration::prediction() {
	return this->prediction_;
}

void SVMConfiguration::set_prediction(bool prediction) {
	this->prediction_ = prediction;
}

void SVMConfiguration::set_weights( Rcpp::NumericVector weights ) {
	this->nr_weight_ = weights.size();
	this->weight_ = new double[nr_weight_];

	for (int i = 0; i < nr_weight_; i++) {
		weight_[i] = weights(1);
	}
}

void SVMConfiguration::set_library( std::string library ) {
	if ( library == "libsvm" ) {
		this->library_ = kLibSMV;
		this->svm_type_ = C_SVC;
	}
	else if (library == "svmlight" ) {
    this->library_ = kSVMLight;
    this->svm_type_ = C_SVC;
	}
}

void SVMConfiguration::set_kernel( std::string kernel ) {
	if ( kernel == "linear" ) {
		this->kernel_type_ = kLinear;
	}
	else if ( kernel == "poly" ) {
		this->kernel_type_ = kPoly;
	}
	else if ( kernel == "rbf" ) {
		this->kernel_type_ = kRBF;
	}
	else if ( kernel == "sigmoid" ) {
		this->kernel_type_ = kSigmoid;
	}
}

void SVMConfiguration::set_preprocess( std::string preprocess ) {
	if ( preprocess ==  "2e" ) {
		this->preprocess_ = kTwoE;
	}
	else if ( preprocess == "none" ) {
		this->preprocess_ = kNone;
	}
}

void SVMConfiguration::set_verbosity(int verbosity){
  this->log_.verbosity_ = verbosity;
}

double SVMConfiguration::b() {
  return -rho_[0];
}

void SVMConfiguration::set_default_params() {
	library_ = kLibSMV;
	svm_type_ = C_SVC;
	kernel_type_ = kLinear;
  preprocess_ = kNone;
	degree_ = 3;
	gamma_ = 0;	// 1/num_features
	coef0_ = 0;
	cache_size_ = 100;
	c_ = 1;
	eps_ = 1e-3;
	shrinking_ = 1;
	probability_ = 0;
	nr_weight_ = 0;
	weight_label_ = NULL;
	weight_ = NULL;
	cov_eps_smoothing_ = 1.0e-10;
//	Probably not necessery
	nu_ = 0.5;
	p_ = 0.1;

    // User-defined classification mode labels
    // (will be filled during data processing)
    label_negative_ = 0;
    label_positive_ = 0;
}

