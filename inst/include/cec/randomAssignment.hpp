#ifndef RANDOMASSIGNMENT_HPP
#define RANDOMASSIGNMENT_HPP

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "assignment.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

namespace gmum {

void initAssignRandom(std::vector<unsigned int> &assignment,
		const arma::mat &points, unsigned int nrOfClusters);

class RandomAssignment: public Assignment {
public:
	RandomAssignment(const arma::mat &points, const int nrOfClusters) :
			Assignment::Assignment(points, nrOfClusters) {
	}

	virtual void operator()(std::vector<unsigned int> &assignment);

};

}

#endif

