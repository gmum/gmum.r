#include <cmath>
#include "two_e_svm_pre.h"

#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_utils.h"
#include <limits>
#include <iostream>

using namespace arma;
using namespace std;

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
    
    //norm type 1, sampling from population, cause cec is using it :-(
    mat cov_pos, cov_neg;
    int max_rows = pos.n_cols;
    if(pos.n_rows == 1) {
      cov_pos = zeros(max_rows, max_rows);
    } else { 
      cov_pos = cov(pos);
    }
    if(neg.n_rows == 1) {
      cov_neg = zeros(max_rows, max_rows);
    } else { 
      cov_neg = cov(neg);
    }
    //norm type 1, sampling from population, cause cec is using it :-(

    mat cov0 =  cov_pos + cov_neg;
    //cov0 +=  data.cov_eps_smoothing_start * arma::eye(data.data.n_cols, data.data.n_cols);
		SvmUtils::sqrtInvMat(cov0, cov0InvSqrt, data.cov_eps_smoothing_start);
    data.inv_of_sqrt_of_cov = cov0InvSqrt;
		mat X_dash_plus = cov0InvSqrt * pos.t();
		mat X_dash_minus = cov0InvSqrt * neg.t();;
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
