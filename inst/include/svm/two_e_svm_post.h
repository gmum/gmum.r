#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>

class TwoeSVMPostprocessor: public SVMHandler {
private:
	arma::rowvec projectingData(arma::mat &matrix, arma::rowvec &weights);
	void makePreprocessor();
	float shiftingBoundary();
public:
	void processRequest(SVMConfiguration&);
	bool canHandle(SVMConfiguration&);
};
