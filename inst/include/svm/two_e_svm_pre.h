#ifndef TWOESVM_H
#define TWOESVM_H

#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace Rcpp;

// Below is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp
// function (or via the Source button on the editor toolbar)

// For more on using Rcpp click the Help button on the editor toolbar

//// [[Rcpp::export]]
//2eSVM Preprocessor
class TwoeSVMPreprocessor: public SVMHandler {
protected:
    arma::mat covPosMat;
    arma::mat covNegMat;
    arma::mat postiveMat;
    arma::mat negtiveMat;
    arma::mat transMat;
    arma::mat preprocessorPos;
    arma::mat preprocessorNeg;

    arma::mat posMat(arma::mat &matrix);
    arma::mat negMat(arma::mat &matrix);
    arma::mat computeTransMat(arma::mat &covPosMat, arma::mat &covNegMat);
    arma::mat mappingPos(arma::mat transMatrix, arma::mat posMatrix);
    arma::mat mappingNeg(arma::mat transMatrix, arma::mat negMatrix);
    arma::mat sqrtMat(arma::mat &matrix);

public:
    void processRequest(SVMConfiguration&);
    bool canHandle(SVMConfiguration&);

    void makePreprocessor();
};

#endif
