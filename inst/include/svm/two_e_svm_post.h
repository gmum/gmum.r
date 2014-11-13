
#ifndef __TWOESVM_H_POST__
#define __TWOESVM_H_POST__

#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>

class TwoeSVMPostprocessor: public SVMHandler {
public:
  arma::rowvec projectingData(arma::mat &matrix, arma::rowvec &weights);
	void processRequest(SVMConfiguration&);
	bool canHandle(SVMConfiguration&);
};

#endif