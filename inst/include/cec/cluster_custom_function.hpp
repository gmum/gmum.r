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

namespace gmum {

class ClusterCustomFunction: public ClusterUseCovMat {
private:
    std::string m_function_name;
    void calculate_entropy();
public:
    ClusterCustomFunction(int count, const arma::rowvec& mean, const arma::mat& cov_mat, const std::string& function_name);
    ClusterCustomFunction(unsigned int id,const std::vector<unsigned int> &assignment, const arma::mat &points, const std::string &function_name);

    virtual double calculate_entropy(int n){
    	throw "Not implemented";
    	return 0.;
    }
    
    virtual ClusterCustomFunction* clone(); 
};

}
#endif  //  CLUSTER_CUSTOM_FUNCTION_HPP
