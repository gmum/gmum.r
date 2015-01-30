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

void TwoeSVMPreprocessor::processRequest(SVMConfiguration& data) {
  data.tmp_data = data.data;
  data.tmp_target = data.target;

  if (!data.prediction) {
  		double targets[2] = {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
  		for(unsigned i = 0; i < data.target.n_rows; i++) {
  			double elem = data.target(i, 0);
  			if(targets[0] == std::numeric_limits<double>::infinity())
  				targets[0] =  elem;
  			else if(targets[0] != elem) {
  				if(targets[1] == std::numeric_limits<double>::infinity()) {
  					targets[1] = elem;
  				} else {
  					if(targets[1] != elem) {
  						data.error_msg = "Class should have only 2 elements";
  						ASSERT(targets[1] != elem);
  						return;
  					} 
  				}
  			}
  		}

  	data.pos_target = targets[0] > targets[1] ? targets[0] : targets[1];
  	data.neg_target = targets[0] > targets[1] ? targets[1] : targets[0];
		mat pos = SvmUtils::matrixByValue(data.data, data.target, data.pos_target);
		mat neg = SvmUtils::matrixByValue(data.data, data.target, data.neg_target);
    mat cov0 = cov(pos) + cov(neg);
    cov0 +=  data.cov_eps_smoothing_start * arma::eye(data.data.n_cols, data.data.n_cols);
		SvmUtils::sqrtInvMat(cov0, cov0InvSqrt, data.cov_eps_smoothing_start);
    data.inv_of_sqrt_of_cov = cov0InvSqrt;
		mat X_dash_plus = cov0InvSqrt * pos.t();
		mat X_dash_minus = cov0InvSqrt * neg.t();
		vec pos_target_vec = vec(pos.n_rows);
		vec neg_target_vec = vec(neg.n_rows);
    pos_target_vec.fill(data.pos_target);
		neg_target_vec.fill(data.neg_target);
		data.target = join_vert(pos_target_vec, neg_target_vec);
		data.data = join_vert(X_dash_plus.t(), X_dash_minus.t());
  } else {
		data.data =  data.data * cov0InvSqrt.t() ;
	}
}

bool TwoeSVMPreprocessor::canHandle(SVMConfiguration& data) {
	// TODO
	return true;
}