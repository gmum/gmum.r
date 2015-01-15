#include <cmath>
#include <RcppArmadillo.h>
#include "two_e_svm_pre.h"

#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_utils.h"
#include <limits>

using namespace arma;
using namespace std;
// template<class Matrix>
void print_matrix(arma::mat matrix) {
    matrix.print(std::cout);
}

mat multi_matrix(arma::mat A, arma::mat B) {
    return A * B;
}

mat invert_matrix(arma::mat A) {
    return inv(A);
}


//provide explicit instantiations of the template function for 
//every matrix type you use somewhere in your program.
// template void print_matrix<arma::mat>(arma::mat matrix);
// template void print_matrix<arma::cx_mat>(arma::cx_mat matrix);

void TwoeSVMPreprocessor::ProcessRequest(SVMConfiguration& data) {
  data.tmp_data_ = data.data_;
  data.tmp_target_ = data.target_;

  if (!data.prediction_) {
  		double targets[2] = {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
  		for(unsigned i = 0; i < data.target_.n_rows; i++) {
  			double elem = data.target_(i, 0);
  			if(targets[0] == std::numeric_limits<double>::infinity())
  				targets[0] =  elem;
  			else if(targets[0] != elem) {
  				if(targets[1] == std::numeric_limits<double>::infinity()) {
  					targets[1] = elem;
  				} else {
  					if(targets[1] != elem) {
  						data.error_msg_ = "Class should have only 2 elements";
  						ASSERT(targets[1] != elem);
  						return;
  					} 
  				}
  			}
  		}

  	data.pos_target_ = targets[0] > targets[1] ? targets[0] : targets[1];
  	data.neg_target_ = targets[0] > targets[1] ? targets[1] : targets[0];
		mat pos = SvmUtils::MatrixByValue(data.data_, data.target_, data.pos_target_);
		mat neg = SvmUtils::MatrixByValue(data.data_, data.target_, data.neg_target_);
    mat cov0 = cov(pos) + cov(neg) + data.cov_eps_smoothing_ * arma::eye(data.data_.n_cols, data.data_.n_cols);
		SvmUtils::SqrtInvMat(cov0, cov0_inv_sqrt);
    data.inv_of_sqrt_of_cov_ = cov0_inv_sqrt;
		mat X_dash_plus = cov0_inv_sqrt * pos.t();
		mat X_dash_minus = cov0_inv_sqrt * neg.t();
		vec pos_target_vec = vec(pos.n_rows);
		vec neg_target_vec = vec(neg.n_rows);
    pos_target_vec.fill(data.pos_target_);
		neg_target_vec.fill(data.neg_target_);
		data.target_ = join_vert(pos_target_vec, neg_target_vec);
		data.data_ = join_vert(X_dash_plus.t(), X_dash_minus.t());
  } else {
		data.data_ =  data.data_ * cov0_inv_sqrt.t() ;
	}
}

bool TwoeSVMPreprocessor::CanHandle(SVMConfiguration& data) {
	// TODO
	return true;
}