#ifndef CLUSTERCUSTOMFUNCTION_HPP
#define CLUSTERCUSTOMFUNCTION_HPP

#include <RcppArmadillo.h>
#include <cmath>
#include <Rcpp.h>
#include <string>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "cluster.hpp"
#include "exceptions.hpp"

namespace gmum {

class ClusterCustomFunction: public ClusterUseCovMat {
private:
	std::string functionName;
	void calculateEntropy();
	boost::shared_ptr<ClusterUseCovMat> createInstance(int, const arma::rowvec&,
			const arma::mat&);
public:
	ClusterCustomFunction(int, const arma::rowvec&, const arma::mat&,
			const std::string&);
	ClusterCustomFunction(unsigned int, const std::vector<unsigned int>&,
			const arma::mat&, const std::string&);
};

}
#endif  //  CLUSTER_CUSTOM_FUNCTION_HPP
