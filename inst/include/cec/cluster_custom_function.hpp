#ifndef CLUSTERCUSTOMFUNCTION_HPP
#define CLUSTERCUSTOMFUNCTION_HPP

#include <armadillo>
#include <cmath>
#include <string>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "cluster.hpp"
#include "exceptions.hpp"

namespace gmum {

class ClusterCustomFunction: public ClusterUseCovMat {
private:
    std::string m_function_name;
    void calculate_entropy();
    boost::shared_ptr<ClusterUseCovMat> create_instance(int count, const arma::rowvec& mean, const arma::mat& mat);
public:
    ClusterCustomFunction(int count, const arma::rowvec& mean, const arma::mat& cov_mat, const std::string& function_name);
    ClusterCustomFunction(unsigned int id,const std::vector<unsigned int> &assignment, const arma::mat &points, const std::string &function_name);
};

}
#endif  //  CLUSTER_CUSTOM_FUNCTION_HPP
