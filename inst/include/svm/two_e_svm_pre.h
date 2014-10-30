#ifndef TWOESVM_H
#define TWOESVM_H

#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>


//2eSVM Preprocessor
class TwoeSVMPreprocessor: public SVMHandler {
private:
    arma::mat computeTransMat(arma::mat &covPosMat, arma::mat &covNegMat);
    void sqrtInvMat(const arma::mat &matrix, arma::mat &finalMat);
    arma::mat cov0InvSqrt;

public:
    void processRequest(SVMConfiguration&);
    bool canHandle(SVMConfiguration&);
    void makePreprocessor();
};

#endif
