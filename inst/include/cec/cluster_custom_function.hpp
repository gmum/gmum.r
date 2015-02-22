#ifndef CLUSTERCUSTOMFUNCTION_HPP
#define CLUSTERCUSTOMFUNCTION_HPP

#include <armadillo>
#include <cmath>
#include <string>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "cluster.hpp"
#include "exceptions.hpp"

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif



#ifdef RCPP_INTERFACE
namespace gmum {
class ClusterCustomFunction: public ClusterUseCovMat {
private:
    boost::shared_ptr<Rcpp::Function> m_function;
	double calculate_entropy(int n, const arma::mat &cov_mat);
public:
	ClusterCustomFunction(int count, const arma::rowvec& mean,
            const arma::mat& cov_mat, boost::shared_ptr<Rcpp::Function> function);
	ClusterCustomFunction(unsigned int id,
			const std::vector<unsigned int> &assignment,
            const arma::mat &points, boost::shared_ptr<Rcpp::Function> function);
    virtual ClusterCustomFunction* clone(); 
};

}

#endif



#endif  //  CLUSTER_CUSTOM_FUNCTION_HPP
