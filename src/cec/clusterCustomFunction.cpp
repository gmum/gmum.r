#include "clusterCustomFunction.hpp"

namespace gmum {
  ClusterCustomFunction::ClusterCustomFunction(int _count,
                                               const arma::rowvec& _mean,
                                               const arma::mat& _covMat,
                                               const std::string& _functionName)
      :ClusterUseCovMat(_count, _mean, _covMat), functionName(_functionName) {
    calculateEntropy();
  }

  ClusterCustomFunction::ClusterCustomFunction(
      unsigned int _id, const std::vector<unsigned int> &_assignment,
      const arma::mat &_points, const std::string &_functionName):
      ClusterUseCovMat(_id, _assignment, _points), functionName(_functionName) {
    calculateEntropy();
  }

  void ClusterCustomFunction::calculateEntropy() {
    Rcpp::Environment myEnv = Rcpp::Environment::global_env();
    Rcpp::Function myFunction = myEnv[functionName];
    _entropy = Rcpp::as<double>(myFunction(Rcpp::Named("m", Rcpp::wrap(mean)),
    Rcpp::Named("sigma", Rcpp::wrap(covMat))));
  }
  boost::shared_ptr<ClusterUseCovMat> ClusterCustomFunction::createInstance(
      int _count, const arma::rowvec& _mean, const arma::mat& _mat) {
    return boost::shared_ptr<ClusterUseCovMat>(new ClusterCustomFunction(
        _count, _mean, _mat, functionName));
  }
}


