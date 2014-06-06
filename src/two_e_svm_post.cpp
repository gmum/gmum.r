/*
 * twoeSVMPostprocessor.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: sacherus
 */

#include <cmath>
#include <RcppArmadillo.h>

#include "two_e_svm_post.h"
#include "svm_handler.h"
#include "svm_basic.h"

void TwoeSVMPostprocessor::processRequest(SVMConfiguration &data) {

}

bool TwoeSVMPostprocessor::canHandle(SVMConfiguration& data) {
	return true;
}

arma::rowvec TwoeSVMPostprocessor::projectingDataPos(arma::mat &matrix,
		arma::rowvec &weights) {
	return weights.t() * matrix;
}

arma::rowvec TwoeSVMPostprocessor::projectingDataNeg(arma::mat &matrix,
		arma::rowvec &weights) {
	return weights.t() * matrix;
}

//float TwoeSVMPostprocessor::shiftingBoundary(arma::rowvec &P1,
//		arma::rowvec &P2) {
//return b-( stddev(P1)-stddev(P2)/stddev(P1)+stddev(P2) )
//
//}

