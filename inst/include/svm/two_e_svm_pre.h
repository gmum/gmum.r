#ifndef __TWOESVM_H_PRE__
#define __TWOESVM_H_PRE__

#include "svm_handler.h"
#include "svm_basic.h"
#include <RcppArmadillo.h>


//2eSVM Preprocessor
class TwoeSVMPreprocessor: public SVMHandler {
private:
    arma::mat ComputeTransMat(arma::mat &cov_pos_mat, arma::mat &cov_neg_mat);
    void SqrtInvMat(const arma::mat &matrix, arma::mat &final_mat);


public:
    void ProcessRequest(SVMConfiguration&);
    bool CanHandle(SVMConfiguration&);
    void MakePreprocessor();
    arma::mat cov0_inv_sqrt;
};

#endif