#include <vector>
#include <list>
#include <armadillo>
#include <time.h>
#include <stdlib.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#ifndef KMEANSPP_ASSIGMENT_HPP
#define KMEANSPP_ASSIGMENT_HPP

namespace gmum {

  struct Pair {
    unsigned int pointNumber;
    float distance;
    Pair(unsigned int n) : pointNumber(n), distance(0) {}
  };

  void initAssignKmeanspp(std::vector<unsigned int> &assignment,
			  arma::mat &points,
			  unsigned int nrOfPoints,
			  unsigned int nrOfClusters);    

  void calculateDistance(std::vector<unsigned int> &centers,
			 std::list<Pair> &selected,
			 arma::mat &points);

  std::list<Pair>::iterator choose(boost::random::bernoulli_distribution<> &bernoulli,
			      boost::random::mt19937 &gen,
			      std::list<Pair> &selected);

  unsigned int findNearest(unsigned int i, 
			   std::vector<unsigned int> &centers,
			   arma::mat &points);

}

#endif
