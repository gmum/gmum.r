#ifndef CLUSTER_CUSTOM_FUNCTION_HPP_
#define CLUSTER_CUSTOM_FUNCTION_HPP_

#include <armadillo>
#include <vector>
#include <cmath>
#include <string>
#include <boost/smart_ptr.hpp>
#include <Rcpp.h>
#include "exceptions.hpp"
#include "Cluster.hpp"



namespace gmum {
  class ClusterCustomFunction : public Cluster {
   private:
    std::string functionName;
    void calculateEntropy();
    boost::shared_ptr<Cluster> createInstance(int,
                                             const arma::rowvec&,
                                             const arma::mat&);
   public:
    ClusterCustomFunction(int,
                          const arma::rowvec&,
                          const arma::mat&,
                          const std::string&);
    ClusterCustomFunction(unsigned int,
                          const std::vector<unsigned int>&,
                          const arma::mat&,
                          const std::string&);
  };

}
#endif  //  CLUSTER_CUSTOM_FUNCTION_HPP_
