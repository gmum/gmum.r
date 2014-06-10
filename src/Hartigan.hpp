#include <RcppArmadillo.h>
#include <vector>
#include <limits>
#include <cmath>
#include "Cluster.hpp"
#include "Algorithm.hpp"

#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP



namespace gmum {

  class Hartigan : public Algorithm {
    void removeCluster(unsigned int source, arma::mat &points, std::vector<unsigned int> &assignment,
		       std::vector<boost::shared_ptr<Cluster> > &clusters);
  public:
    Hartigan(bool logNrOfClusters, bool logEnergy);
    TotalResult loop(arma::mat &points, std::vector<unsigned int> &assignment,
		       double killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters);
    SingleResult singleLoop(arma::mat &points, std::vector<unsigned int> &assignment, 
			      double killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters);
  };

}

#endif
