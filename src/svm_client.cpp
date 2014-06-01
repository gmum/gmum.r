#include "svm_client.h"

// Constructor
SVMClient::SVMClient(SVMConfiguration *config) {
	SVMConfiguration current_config = *config;
	this->config = current_config;
}

void SVMClient::setX( arma::mat x ){
	config.data = x;
}
void SVMClient::setY( arma::vec y ){
	config.target = y;
}

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
	case LIBSVM : return "libsvm"; break;
	}
}
std::string SVMClient::getKernel(){
	switch(config.kernel_type) {
	case _LINEAR : return "linear"; break;
	case _POLY : return "poly"; break;
	case _RBF : return "rbf"; break;
	case _SIGMOID : return "sigmoid"; break;
	}
}
std::string SVMClient::getPreprocess() {
	switch(config.preprocess) {
	case TWOE : return "2e"; break;
	case NONE : return "none"; break;
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



// Main client function, it uses FlowFactory to get a certain work flow
// and runs processRequest(c,r) function on each block returing last result
void SVMClient::run() {
	SVMClient::createFlow();
	for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
			iter != SVMHandlers.end(); ++iter) {
		(*iter)->processRequest(config);
	}
}

void SVMClient::predict( arma::mat problem ) {
	config.data = problem;
	if ( SVMHandlers.size() > 0 ) {
		config.setPrediction(true);
		for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
				iter != SVMHandlers.end(); ++iter) {
			(*iter)->processRequest(config);
		}
	}
}

void SVMClient::createFlow() {
	SVMType svm_type = config.svm_type;
	Preprocess preprocess = config.preprocess;
	std::vector<SVMHandler*> handlers;

	//	switch (svm_type) {
	//		case LIBSVM: {
	//			LibSVMRunner runner;
	//			handlers.push_back(&runner);
	//			break;
	//		}
	//		case SVMLIGHT : {	SVMLightRunner runner;		// Wating for svm light runner implementation
	//			handlers.push_back( &runner );
	//			break;
	//		}
	//		default: {
	//			LibSVMRunner runner;				// dafault will be libsvm
	//			handlers.push_back(&runner);
	//			break;
	//		}
	//	}

	switch (preprocess) {
	// case TWOE :	{	TwoeSVMPostprocessor post_runner;
	// 				TwoeSVMPreprocessor pre_runner;
	// 				handles.insert( handlers.bedin(), pre_runner );
	// 				handlers.push_back( post_runner );
	// 				break;
	// 			}

	case VK:
		break; // TODO

	case NONE:
		break;

	case NORM: {
		NormRunner norm_runner;
		handlers.push_back(&norm_runner);
		break;
	}
	default:
		break;
	}

	this->SVMHandlers = handlers;
}

