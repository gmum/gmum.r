#include <RcppArmadillo.h>
#include <vector>
#include <limits>
#include <cmath>
#include "cluster.hpp"
#include "algorithm.hpp"

#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP



namespace gmum {

  class Hartigan : public Algorithm {
    void removeCluster(unsigned int source, const arma::mat &points,
		       std::vector<unsigned int> &assignment,
		       std::vector<boost::shared_ptr<Cluster> > &clusters);
    
  public:
    Hartigan(bool logNrOfClusters, bool logEnergy);
    TotalResult loop(const arma::mat &points, std::vector<unsigned int> &assignment,
		       double killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters);
    SingleResult singleLoop(const arma::mat &points, std::vector<unsigned int> &assignment, 
			      double killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters);
    double entropy(boost::shared_ptr<Cluster> ptrToCluster, int numberOfPoints);
  };

}

#endif
