#include <vector>
#include <RcppArmadillo.h>
#include <time.h>
#include <stdlib.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "assignment.hpp"

#ifndef RANDOM_ASSIGMENT_HPP
#define RANDOM_ASSIGMENT_HPP

namespace gmum {

  void initAssignRandom(std::vector<unsigned int> &assignment,
			const arma::mat &points,
			unsigned int nrOfClusters);

}

#endif

