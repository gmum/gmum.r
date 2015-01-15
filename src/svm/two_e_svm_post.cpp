#include <cmath>
#include <RcppArmadillo.h>

#include "two_e_svm_post.h"
#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_utils.h"

using namespace arma;
using namespace std;

void TwoeSVMPostprocessor::ProcessRequest(SVMConfiguration &data) {
  if (!data.prediction_) {
		// int dim = data.data.n_cols;
		// cout << "SVs: " << data.l << endl;
		// cout << "dim: " << dim << endl;
		// cout << "Classes: " << data.nr_class << endl;
		// for(int i = 0; i < data.nr_class; i++) {
		// 	cout << "Class: " << i << " SVs: " << data.nSV[i];

		//TODO: Use copying memory for better memory optimalization
		//DIM PROJECTION: examps x dim x dim x 1 = exams x 1
    data.w_ = data.inv_of_sqrt_of_cov_.t() * data.w_;
		double p_plus = stddev(SvmUtils::MatrixByValue(data.data_, data.target_, data.pos_target_) * data.w_);
		double p_minus = stddev(SvmUtils::MatrixByValue(data.data_, data.target_, data.neg_target_) * data.w_);
    
		//-rho = b
		//TODO: consider multiclass examples
		double b_dash = data.b() + (p_plus - p_minus) / (p_plus + p_minus);
		data.rho_[0] = -b_dash;
	}
	data.data_ = data.tmp_data_;
	data.target_ = data.tmp_target_;
}

bool TwoeSVMPostprocessor::CanHandle(SVMConfiguration& data) {
	return true;
}