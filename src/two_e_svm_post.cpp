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
#include "svm_utils.h"

using namespace arma;
void TwoeSVMPostprocessor::processRequest(SVMConfiguration &data) {
	if (!data.prediction) {
		int dim = data.data.n_cols;
		vec arma_sv = SvmUtils::libtoarma(data.SV, data.l, dim);
		vec arma_coefs = SvmUtils::arrtoarma(data.sv_coef, data.l);
		//Xk is already transposed
		//examples x dim
		//DIM W: (nsv x 1)^T x nsv x dim = 1 x dim
		vec w = arma_coefs.t() * arma_sv;
		w = w.t();
		//TODO: SET POSITIVE AND NEGATIVE MATRIX
		//DIM PROJECTION: examps x dim x dim x 1 = exams x 1
		double p_plus = stddev(data.data * w);
		double p_minus = stddev(data.data * w);
		//-rho = b
		//TODO: consider multiclass examples
		double b_dash = -(data.rho[0]) - (p_plus - p_minus) / (p_plus + p_minus);
		data.rho[0] = -b_dash;
	}
}

bool TwoeSVMPostprocessor::canHandle(SVMConfiguration& data) {
	return true;
}

