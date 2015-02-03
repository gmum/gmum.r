#include "svm_client.h"
#include "libsvm_runner.h"
#include "svmlight_runner.h"
#include "two_e_svm_pre.h"
#include "two_e_svm_post.h"
#include "svm_utils.h"

const std::string __file__ = "svm_client.cpp";
const std::string __client_class__ = "SVMClient";
const std::string __debug_prefix__ = __file__ + "." + __client_class__;


// Constructor
SVMClient::SVMClient(SVMConfiguration *config): config(*config) {}

// Setters
void SVMClient::setX( arma::mat x ){
	config.data = x;
}
void SVMClient::setY( arma::vec y ){
	config.target = y;
}

void SVMClient::setLibrary(std::string library){
	config.setLibrary(library);
}
void SVMClient::setKernel(std::string kernel){
	config.setKernel(kernel);
}
void SVMClient::setPreprocess(std::string prep){
	config.setPreprocess(prep);
}

void SVMClient::setCacheSize(double cache) {
	config.cache_size = cache;
}
void SVMClient::setDegree(int degree){
	config.degree = degree;
}
void SVMClient::setGamma(double gamma){
	config.gamma = gamma;
}
void SVMClient::setCoef0(double coef0 ){
	config.coef0 = coef0;
}
void SVMClient::setC(double C){
	config.C = C;
}
void SVMClient::setEps(double eps){
	config.eps = eps;
}
void SVMClient::setShrinking(int sh){
	config.shrinking = sh;
}
void SVMClient::setProbability(int prob){
	config.probability = prob;
}

void SVMClient::setConfiguration(SVMConfiguration *config) {
	SVMConfiguration current_config = *config;
	this->config = current_config;
}

// Getters
arma::mat SVMClient::getX(){
	return config.data;
}
arma::vec SVMClient::getY(){
	return config.target;
}
arma::vec SVMClient::getPrediction() {
	return SVMClient::config.result;
}
std::string SVMClient::getLibrary(){
	switch(config.svm_type) {
	case LIBSVM : return "libsvm";
    case SVMLIGHT : return "svmlight";
    default : return "error"; 
	}
}
std::string SVMClient::getKernel(){
	switch(config.kernel_type) {
	case _LINEAR : return "linear";
	case _POLY : return "poly"; 
	case _RBF : return "rbf"; 
	case _SIGMOID : return "sigmoid"; 
  default : return "error"; 
	}
}
std::string SVMClient::getPreprocess() {
	switch(config.preprocess) {
	case TWOE : return "2e";
	case NONE : return "none";
  default : return "error";
	}
}
double SVMClient::getCacheSize(){
	return config.cache_size;
}
int SVMClient::getDegree(){
	return config.degree;
}
double SVMClient::getGamma(){
	return config.gamma;
}
double SVMClient::getCoef0(){
	return config.coef0;
}
double SVMClient::getC(){
	return config.C;
}
double SVMClient::getEps(){
	return config.eps;
}
bool SVMClient::isShrinking(){
	return (bool)config.shrinking;
}
bool SVMClient::isProbability(){
	return (bool)config.probability;
}

// model getters
arma::vec SVMClient::getAlpha() {
	return config.alpha_y;
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

void SVMClient::setBias(double bias) {
	config.setB(bias);
}

double SVMClient::getBias() {	
	return config.getB();		
}

arma::vec SVMClient::getW() {		
	if ( config.kernel_type == _LINEAR ) {
		return config.w;
	}
	else {
    LOG(config.log, LogLevel::ERR, "ERROR: " + to_string("Decision boundary is not available with non-linear kernel"));
		return 0;
	}
}

int SVMClient::get_number_sv() {
  return config.support_vectors.n_rows;
}

int SVMClient::get_number_class() {
  return config.nr_class;
}

arma::mat SVMClient::getSV(){
  return config.support_vectors;
}

SVMConfiguration SVMClient::getConfiguration() {
    return this->config;
}

// Runners
void SVMClient::run() {
	SVMClient::createFlow();
	for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
			iter != SVMHandlers.end(); ++iter) {
		(*iter)->processRequest(config);
	}
}

void SVMClient::train() {
	config.setPrediction(false);
	run();
}

void SVMClient::predict( arma::mat problem ) {
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".predict() Started.");

    // FIXME: Calculate TwoE prediction in this method
    if (config.kernel_type != _LINEAR || config.preprocess == TWOE) {
        // Request prediction from handlers when not gmum.r-supported
        // NOTE: Currently LINEAR only
        requestPredict(problem);
        return;
    }

    // Just like in requestPredict()
	config.setData(problem);

    // Number of docs is a number of rows in data matrix
    size_t n_docs = config.data.n_rows;

    config.result = arma::randu<arma::vec>(n_docs);

    // Linear kernel
    for (int i=0; i < n_docs; ++i) {
        double doc_result = 0;
        // For every support vector
        for (int j=0; j < config.support_vectors.n_rows; ++j) {
            double sum_j = arma::dot(
                config.data.row(i),
                config.support_vectors.row(j)
            );
            sum_j *= config.alpha_y(j);
            doc_result += sum_j;
        }
        doc_result += config.threshold_b;
        config.result[i] = doc_result;
    }

    // Convert results to userdefined labels
    n_docs = config.result.n_rows;
    double doc_result = 0;
    for (int i=0; i < n_docs; ++i) {
        doc_result = config.result[i];

        // Store user-defined label
        if (doc_result < 0) {
            config.result[i] = config.neg_target;
        } else if (doc_result > 0) {
            config.result[i] = config.pos_target;
        } else {
            config.result[i] = 0;
        }
    }

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".predict() Done."
    );
}

void SVMClient::requestPredict( arma::mat problem ) {
	config.setData(problem);
	if ( SVMHandlers.size() > 0 ) {
		config.setPrediction(true);
		for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
				iter != SVMHandlers.end(); ++iter) {
			(*iter)->processRequest(config);
		}
	}
}

void SVMClient::createFlow() {
	SVMType svm_type = config.library;
	Preprocess preprocess = config.preprocess;
	std::vector<SVMHandler*> handlers;


		switch (svm_type) {
			case LIBSVM: {
				LibSVMRunner *runner = new LibSVMRunner();
				handlers.push_back(runner);
				break;
			}
			case SVMLIGHT : {	
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
	  case TWOE :	{	
          TwoeSVMPostprocessor *post_runner = new TwoeSVMPostprocessor();
	 				TwoeSVMPreprocessor *pre_runner = new TwoeSVMPreprocessor();
	 				handlers.insert( handlers.begin(), pre_runner );
	 				handlers.push_back( post_runner );
	 				break;
	  }
	case NONE:
		break; 
	default:
		break;
	}

	this->SVMHandlers = handlers;
}

