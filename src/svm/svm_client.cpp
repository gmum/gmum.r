#include "svm_client.h"
#include "libsvm_runner.h"
#include "svmlight_runner.h"
#include "two_e_svm_pre.h"
#include "two_e_svm_post.h"
#include "svm_utils.h"

// Constructor
SVMClient::SVMClient(SVMConfiguration *config) {
	SVMConfiguration current_config = *config;
	this->config_ = current_config;
}

// Setters
void SVMClient::set_x( arma::mat x ){
	config_.data_ = x;
}
void SVMClient::set_y( arma::vec y ){
	config_.target_ = y;
}

void SVMClient::set_library(std::string library){
	config_.set_library(library);
}
void SVMClient::set_kernel(std::string kernel){
	config_.set_kernel(kernel);
}
void SVMClient::set_preprocess(std::string prep){
	config_.set_preprocess(prep);
}

void SVMClient::set_cache_size(double cache) {
	config_.cache_size_ = cache;
}
void SVMClient::set_degree(int degree){
	config_.degree_ = degree;
}
void SVMClient::set_gamma(double gamma){
	config_.gamma_ = gamma;
}
void SVMClient::set_coef0(double coef0 ){
	config_.coef0_ = coef0;
}
void SVMClient::set_c(double C){
	config_.c_ = C;
}
void SVMClient::set_eps(double eps){
	config_.eps_ = eps;
}
void SVMClient::set_shrinking(int sh){
	config_.shrinking_ = sh;
}
void SVMClient::set_probability(int prob){
	config_.probability_ = prob;
}

// Getters
arma::mat SVMClient::x(){
	return config_.data_;
}
arma::vec SVMClient::y(){
	return config_.target_;
}
arma::vec SVMClient::prediction() {
	return SVMClient::config_.result_;
}
std::string SVMClient::library(){
	switch(config_.svm_type_) {
	case kLibSMV : return "libsvm";
    case kSVMLight : return "svmlight";
    default : return "error"; 
	}
}
std::string SVMClient::kernel(){
	switch(config_.kernel_type_) {
	case kLinear : return "linear";
	case kPoly : return "poly"; 
	case kRBF : return "rbf"; 
	case kSigmoid : return "sigmoid"; 
  default : return "error"; 
	}
}
std::string SVMClient::preprocess() {
	switch(config_.preprocess_) {
	case kTwoE : return "2e";
	case kNone : return "none";
  default : return "error";
	}
}
double SVMClient::cache_size(){
	return config_.cache_size_;
}
int SVMClient::degree(){
	return config_.degree_;
}
double SVMClient::gamma(){
	return config_.gamma_;
}
double SVMClient::coef0(){
	return config_.coef0_;
}
double SVMClient::c(){
	return config_.c_;
}
double SVMClient::eps(){
	return config_.eps_;
}
bool SVMClient::shrinking(){
	return (bool)config_.shrinking_;
}
bool SVMClient::probability(){
	return (bool)config_.probability_;
}

// model getters
arma::vec SVMClient::alpha() {
	return arma::vec(config_.rho_,config_.l_);
}

//void SVMClient::setAlpha(double* alpha) {
//  if (config. - length(alpha) == 1) {
//    for (int i = 0; i != length(alpha); i++) {
//      config.rho[i+1] = alpha[i];
//    }
//  } 
//  else if (length(config.rho) != length(alpha)) {
//    LOG(config.log, logLevel::ERR, "ERROR: " + to_string("Wrong alpha array size."));
//    return;
//  }
//  else {
//    config.rho = alpha;
//  }
//}

void SVMClient::set_bias(double bias) {
  config_.rho_[0] = -bias;
}

double SVMClient::bias() {	
	return config_.b();		
}

arma::vec SVMClient::w() {		
	if ( config_.kernel_type_ == kLinear ) {
		return config_.w_;
	}
	else {
    LOG(config_.log_, LogLevel::kError, "ERROR: " + ToString("Decision boundry is not available with non-linear kernel"));
		return 0;
	}
}

int SVMClient::number_sv() {
  return config_.l_;
}

int SVMClient::number_class() {
  return config_.nr_class_;
}

arma::mat SVMClient::sv(){
  return config_.arma_sv_;
}

// Runners
void SVMClient::Run() {
	SVMClient::CreateFlow();
	for (std::vector<SVMHandler*>::iterator iter = svm_handlers_.begin();
			iter != svm_handlers_.end(); ++iter) {
		(*iter)->ProcessRequest(config_);
	}
}

void SVMClient::Train() {
	config_.set_prediction(false);
	Run();
}

void SVMClient::Predict( arma::mat problem ) {
	config_.set_data(problem);
	if ( svm_handlers_.size() > 0 ) {
		config_.set_prediction(true);
		for (std::vector<SVMHandler*>::iterator iter = svm_handlers_.begin();
				iter != svm_handlers_.end(); ++iter) {
			(*iter)->ProcessRequest(config_);
		}
	}
}

void SVMClient::CreateFlow() {
	SVMType svm_type = config_.library_;
	Preprocess preprocess = config_.preprocess_;
	std::vector<SVMHandler*> handlers;


		switch (svm_type) {
			case kLibSMV: {
				LibSVMRunner *runner = new LibSVMRunner();
				handlers.push_back(runner);
				break;
			}
			case kSVMLight : {	
        SVMLightRunner *runner = new SVMLightRunner();		// Wating for svm light runner implementation
				handlers.push_back(runner);
				break;
			}
			default: {
				LibSVMRunner *runner = new LibSVMRunner();				// dafault will be libsvm
				handlers.push_back(runner);
				break;
			}
		}

	switch (preprocess) {
	  case kTwoE :	{	
          TwoeSVMPostprocessor *post_runner = new TwoeSVMPostprocessor();
	 				TwoeSVMPreprocessor *pre_runner = new TwoeSVMPreprocessor();
	 				handlers.insert( handlers.begin(), pre_runner );
	 				handlers.push_back( post_runner );
	 				break;
	  }
	case kNone:
		break; 
	default:
		break;
	}

	this->svm_handlers_ = handlers;
}

