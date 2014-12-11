#include "svm_client.h"

// Constructor
SVMClient::SVMClient(SVMConfiguration *config) {
	SVMConfiguration current_config = *config;
	this->config = current_config;
	trained = false;
}

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
	}
}
std::string SVMClient::getKernel(){
	switch(config.kernel_type) {
	case _LINEAR : return "linear"; 
	case _POLY : return "poly"; 
	case _RBF : return "rbf";
	case _SIGMOID : return "sigmoid";
	}
}
std::string SVMClient::getPreprocess() {
	switch(config.preprocess) {
	case TWOE : return "2e"; 
	case NONE : return "none";
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
double* SVMClient::getAlpha() {
	return config.rho;
}

double SVMClient::getBias() {	// where is the bias in config?
	return 0.0;			// temporary
}

double* SVMClient::getW() {		// where is W in config?
	if ( config.kernel_type == _LINEAR ) {
		return new double[2];	// temporary
	}
	else {
		return 0;
	}
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
	trained = true;
}

void SVMClient::predict( arma::mat problem ) {
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
	//		case SVMLIGHT : {	SVMLightRunner runner;		// Wating for svm light runner implementation
	//			handlers.push_back( &runner );
	//			break;
	//		}
			default: {
				LibSVMRunner *runner = new LibSVMRunner();				// dafault will be libsvm
				handlers.push_back(runner);
				break;
			}
		}

	switch (preprocess) {
	// case TWOE :	{	TwoeSVMPostprocessor post_runner;
	// 				TwoeSVMPreprocessor pre_runner;
	// 				handles.insert( handlers.bedin(), pre_runner );
	// 				handlers.push_back( post_runner );
	// 				break;
	// 			}

	case NONE:
		break;
    
	default:
		break;
	}

	this->SVMHandlers = handlers;
}

