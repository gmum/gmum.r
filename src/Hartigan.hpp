#include <armadillo>
#include <vector>
#include <limits>
#include <cmath>
#include "Cluster.hpp"
#include "Algorithm.hpp"

#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP

class Hartigan : public Algorithm {
public:
  Hartigan(bool logNrOfClusters, bool logEnergy);
  TotalResult loop(arma::mat &points, std::vector<unsigned int> &assignment,
		   float killThreshold, std::vector<Cluster> &clusters);
  SingleResult singleLoop(arma::mat &points, std::vector<unsigned int> &assignment, 
			  float killThreshold, std::vector<Cluster> &clusters);
};

#endif
