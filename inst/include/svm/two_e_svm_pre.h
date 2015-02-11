#ifndef __TWOESVM_H_PRE__
#define __TWOESVM_H_PRE__

#include "svm_handler.h"
#include "svm_basic.h"
#include <armadillo>


//2eSVM Preprocessor
class TwoeSVMPreprocessor: public SVMHandler {
private:
    arma::mat computeTransMat(arma::mat &covPosMat, arma::mat &covNegMat);
    void sqrtInvMat(const arma::mat &matrix, arma::mat &finalMat);


public:
    void processRequest(SVMConfiguration&);
    bool canHandle(SVMConfiguration&);
    void makePreprocessor();
    arma::mat cov0InvSqrt;
};

#endif
