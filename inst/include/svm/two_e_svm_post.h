
#ifndef __TWOESVM_H_POST__
#define __TWOESVM_H_POST__

#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>

class TwoeSVMPostprocessor: public SVMHandler {
public:
  arma::rowvec ProjectingData(arma::mat &matrix, arma::rowvec &weights);
	void ProcessRequest(SVMConfiguration&);
	bool CanHandle(SVMConfiguration&);
};

#endif