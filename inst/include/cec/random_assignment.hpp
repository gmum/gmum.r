#include <time.h>
#include <stdlib.h>
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <RcppArmadillo.h>
#include "assignment.hpp"

#ifndef RANDOM_ASSIGMENT_HPP
#define RANDOM_ASSIGMENT_HPP

namespace gmum {

  void initAssignRandom(std::vector<unsigned int> &assignment,
			const arma::mat &points,
			unsigned int nrOfClusters);
			
  class RandomAssignment : public Assignment {
  public:
    RandomAssignment(const arma::mat &points, const int nrOfClusters)
      : Assignment::Assignment(points, nrOfClusters) {};
    virtual void operator() (std::vector<unsigned int> &assignment);
  };

}

#endif

