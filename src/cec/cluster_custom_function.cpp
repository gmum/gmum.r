#include "cluster_custom_function.hpp"

namespace gmum {
  ClusterCustomFunction::ClusterCustomFunction(int _count,
                                               const arma::rowvec& _mean,
                                               const arma::mat& _covMat,
                                               const std::string& _functionName): 
      Cluster(_count, _mean, _covMat), functionName(_functionName) {  
  }

  ClusterCustomFunction::ClusterCustomFunction(
      unsigned int _id, const std::vector<unsigned int> &_assignment,
      const arma::mat &_points, const std::string &_functionName):
      Cluster(_id, _assignment, _points), functionName(_functionName) {
  }

  void ClusterCustomFunction::calculateEntropy() {
    // Rcpp::Environment myEnv = Environment::global_env();
    // Rcpp::Function myFunction = myEnv[functionName];
    Rcpp::Language call(functionName, Rcpp::List::create(Rcpp::Named("m", mean),
        Rcpp::Named("sigma", covMat)));
    _entropy = Rcpp::as<double>(call.eval());
  }
  boost::shared_ptr<Cluster> ClusterCustomFunction::createInstance(
      int _count, const arma::rowvec& _mean, const arma::mat& _mat) {
    return boost::shared_ptr<Cluster>(new ClusterCustomFunction(
        _count, _mean, _mat, functionName));
  }
                              
}
