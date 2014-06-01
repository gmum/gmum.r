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

			.field("x", &SVMConfiguration::data)
			.field("y", &SVMConfiguration::target)
			.field("result", &SVMConfiguration::result)

			.field("C", &SVMConfiguration::C)
			.field("gamma", &SVMConfiguration::gamma)
			.field("coef0", &SVMConfiguration::coef0)
			.field("eps", &SVMConfiguration::eps)
			.field("degree", &SVMConfiguration::degree)
			.field("cache_size", &SVMConfiguration::cache_size)
			.field("shrinking", &SVMConfiguration::shrinking)
			.field("probability", &SVMConfiguration::probability)

			.method("createParams", &SVMConfiguration::createParams)
			.method("setPrediction", &SVMConfiguration::setPrediction)
			.method("setWeights", &SVMConfiguration::setWeights)
			.method("setLibrary", &SVMConfiguration::setLibrary)
			.method("setKernel", &SVMConfiguration::setKernel)
			.method("setPreprocess", &SVMConfiguration::setPreprocess)
			;
	class_<SVMClient>("SVMClient")
			.constructor<SVMConfiguration*>()

			.method("setX", &SVMClient::setX)
			.method("setY", &SVMClient::setY)

			.method("getX", &SVMClient::getX)
			.method("getY", &SVMClient::getY)
			.method("getPrediction", &SVMClient::getPrediction)

			.method("getLibrary", &SVMClient::getLibrary)
			.method("getKernel", &SVMClient::getKernel)
			.method("getPreprocess", &SVMClient::getPreprocess)

			.method("getCache", &SVMClient::getCacheSize)
			.method("getDegree", &SVMClient::getDegree)
			.method("getGamma", &SVMClient::getGamma)
			.method("getCoef0", &SVMClient::getCoef0)
			.method("getC", &SVMClient::getC)
			.method("getEps", &SVMClient::getEps)
			.method("isShrinking", &SVMClient::isShrinking)
			.method("isProbability", &SVMClient::isProbability)

			.method("run", &SVMClient::run)
			.method("predict", &SVMClient::predict)
			;

}

#endif
