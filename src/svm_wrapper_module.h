#ifndef SVM_WRAPPER_MODULE_H
#define SVM_WRAPPER_MODULE_H

#include "svm_basic.h"

RCPP_MODULE(svm_wrapper) {
	using namespace Rcpp;

	class_<SVMConfiguration>("SVMConfiguration").constructor().method("setData",
			&SVMConfiguration::setData);

	class_<SVMClient>("SVMClient").constructor().method("run", &SVMClient::run);

}

#endif
