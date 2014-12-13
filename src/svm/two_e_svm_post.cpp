#include <cmath>
#include <RcppArmadillo.h>

#include "two_e_svm_post.h"
#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_utils.h"

using namespace arma;
using namespace std;

void TwoeSVMPostprocessor::processRequest(SVMConfiguration &data) {
  if (!data.prediction) {
		// int dim = data.data.n_cols;
		// cout << "SVs: " << data.l << endl;
		// cout << "dim: " << dim << endl;
		// cout << "Classes: " << data.nr_class << endl;
		// for(int i = 0; i < data.nr_class; i++) {
		// 	cout << "Class: " << i << " SVs: " << data.nSV[i];

		//TODO: Use copying memory for better memory optimalization
		//DIM PROJECTION: examps x dim x dim x 1 = exams x 1
    data.w = data.inv_of_sqrt_of_cov.t() * data.w;
		double p_plus = stddev(SvmUtils::matrixByValue(data.data, data.target, data.pos_target) * data.w);
		double p_minus = stddev(SvmUtils::matrixByValue(data.data, data.target, data.neg_target) * data.w);
		//-rho = b
		//TODO: consider multiclass examples
		double b_dash = data.getB() - (p_plus - p_minus) / (p_plus + p_minus);
		data.rho[0] = -b_dash;
	}
	// data.data = data.tmp_data;
	// data.target = data.tmp_target;
}

bool TwoeSVMPostprocessor::canHandle(SVMConfiguration& data) {
	return true;
}