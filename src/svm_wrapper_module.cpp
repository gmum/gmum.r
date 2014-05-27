#ifndef SVM_WRAPPER_MODULE_H
#define SVM_WRAPPER_MODULE_H

#include "svm_basic.h"
#include "svm_client.h"
using namespace Rcpp;


RCPP_EXPOSED_CLASS(SVMConfiguration)
RCPP_EXPOSED_CLASS(SVMClient)


RCPP_MODULE(svm_wrapper) {
	
	class_<SVMConfiguration>("SVMConfiguration")
			.constructor()
			//.method("setData",&SVMConfiguration::setData)
			.field("data", &SVMConfiguration::data)
			.field("result", &SVMConfiguration::result)
			.field("C", &SVMConfiguration::C)
			.method("createParams", &SVMConfiguration::createParams)
			;
	class_<SVMClient>("SVMClient")
			.constructor<SVMConfiguration*>()
			.method("run", &SVMClient::run)
			.method("getConfig", &SVMClient::getConfig)
			.method("getResult", &SVMClient::getResult)
			;

}

#endif
