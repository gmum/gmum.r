#include <armadillo>
#include <vector>
#include <limits>
#include "Cluster.hpp"
#include "Algorithm.hpp"

#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP

class Hartigan : public Algorithm {
public:
  void loop(arma::mat &points, std::vector<unsigned int> &assignment,
	    float killThreshold, std::vector<Cluster> &clusters);
  int singleLoop(arma::mat &points, std::vector<unsigned int> &assignment, 
		 float killThreshold, std::vector<Cluster> &clusters);
};

#endif
