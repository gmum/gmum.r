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

			.field("x", &SVMConfiguration::data_)
			.field("y", &SVMConfiguration::target_)
			.field("result", &SVMConfiguration::result_)

			.field("C", &SVMConfiguration::c_)
			.field("gamma", &SVMConfiguration::gamma_)
			.field("coef0", &SVMConfiguration::coef0_)
			.field("eps", &SVMConfiguration::eps_)
			.field("degree", &SVMConfiguration::degree_)
			.field("cache_size", &SVMConfiguration::cache_size_)
			.field("shrinking", &SVMConfiguration::shrinking_)
			.field("probability", &SVMConfiguration::probability_)

			.method("setPrediction", &SVMConfiguration::set_prediction)
			.method("setWeights", &SVMConfiguration::set_weights)
			.method("setLibrary", &SVMConfiguration::set_library)
			.method("setKernel", &SVMConfiguration::set_kernel)
			.method("setPreprocess", &SVMConfiguration::set_preprocess)
      .method("set_verbosity", &SVMConfiguration::set_verbosity)
			;
	class_<SVMClient>("SVMClient")
			.constructor<SVMConfiguration*>()

			.method("setX", &SVMClient::set_x)
			.method("setY", &SVMClient::set_y)

			.method("setLibrary",&SVMClient::set_library)
			.method("setKernel", &SVMClient::set_kernel)
			.method("setPreprocess",&SVMClient::set_preprocess)
			.method("setCache",&SVMClient::set_cache_size)
			.method("setDegree",&SVMClient::set_degree)
			.method("setGamma",&SVMClient::set_gamma)
			.method("setCoef0",&SVMClient::set_coef0)
			.method("setC",&SVMClient::set_c)
			.method("setEps",&SVMClient::set_eps)
			.method("setShrinking",&SVMClient::set_shrinking)
			.method("setProbability",&SVMClient::set_probability)
      .method("setBias", &SVMClient::set_bias)

			.method("getX", &SVMClient::x)
			.method("getY", &SVMClient::y)
			.method("getPrediction", &SVMClient::prediction)

			.method("getLibrary", &SVMClient::library)
			.method("getKernel", &SVMClient::kernel)
			.method("getPreprocess", &SVMClient::preprocess)
			.method("getCache", &SVMClient::cache_size)
			.method("getDegree", &SVMClient::degree)
			.method("getGamma", &SVMClient::gamma)
			.method("getCoef0", &SVMClient::coef0)
			.method("getC", &SVMClient::c)
			.method("getEps", &SVMClient::eps)
			.method("isShrinking", &SVMClient::shrinking)
			.method("isProbability", &SVMClient::probability)
      
			.method("getAlpha", &SVMClient::alpha)
			.method("getBias", &SVMClient::bias)
			.method("getW", &SVMClient::w)
      .method("get_number_sv", &SVMClient::number_sv)
      .method("get_number_class", &SVMClient::number_class)
      .method("get_SV", &SVMClient::sv)

			.method("train", &SVMClient::Train)
			.method("predict", &SVMClient::Predict)
			;

}

#endif
