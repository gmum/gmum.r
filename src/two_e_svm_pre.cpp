#include <cmath>
#include <RcppArmadillo.h>
#include "two_e_svm_pre.h"

#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_utils.h"

using namespace arma;
void TwoeSVMPreprocessor::processRequest(SVMConfiguration& data) {
	if (!data.prediction) {
		mat pos = SvmUtils::matrixByValue(data.data, data.target, 1);
		mat neg = SvmUtils::matrixByValue(data.data, data.target, -1);
//		mat cov0InvSqrt;
		sqrtInvMat(cov(pos) + cov(neg), cov0InvSqrt);
		mat X_dash_plus = cov0InvSqrt * pos.t();
		mat X_dash_minus = cov0InvSqrt * neg.t();
		vec pos_target = vec(pos.n_rows);
		pos_target.fill(1);
		vec neg_target = vec(neg.n_rows);
		neg_target.fill(-1);
		data.target = join_vert(pos_target, neg_target);
		data.setData(join_vert(X_dash_plus.t(), X_dash_minus.t()));
//		data.two_e_cov_inv_sqrt = cov0InvSqrt;
	} else {
		data.data =  (cov0InvSqrt* data.data.t()).t();
	}
}

bool TwoeSVMPreprocessor::canHandle(SVMConfiguration& data) {
	// TODO
	return true;
}

void TwoeSVMPreprocessor::sqrtInvMat(const arma::mat &matrix, arma::mat &finalMat) {
	arma::vec eigenValue;
	arma::mat eigenVector;
	arma::mat diagonalMat;

	// Throws runtime decomposition error
	arma::eig_sym(eigenValue, eigenVector, matrix);
	arma::sqrt(eigenVector);
	finalMat = eigenVector.t() * arma::inv(arma::diagmat(eigenVector))
			* eigenVector;
}
