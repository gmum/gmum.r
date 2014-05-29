#include <vector>
#include <time.h>
#include <stdlib.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#ifndef RANDOM_ASSIGMENT_HPP
#define RANDOM_ASSIGMENT_HPP

namespace gmum {

  void initAssignRandom(std::vector<unsigned int> &assignment,
			unsigned int nrOfPoints,
			unsigned int nrOfClusters);

}

#endif

