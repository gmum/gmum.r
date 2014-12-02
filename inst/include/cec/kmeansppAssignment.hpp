#ifndef KMEANSPPASSIGNMENT_HPP
#define KMEANSPPASSIGNMENT_HPP

#include <list>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "assignment.hpp"
#include "boost/random/bernoulli_distribution.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"

namespace gmum {

struct Pair {
	unsigned int pointNumber;
	float distance;
	Pair(unsigned int n) :
			pointNumber(n), distance(0) {
	}
	bool operator<(Pair p) {
		return distance >= p.distance;
	}
};

void initAssignKmeanspp(std::vector<unsigned int> &assignment,
		const arma::mat &points, unsigned int nrOfClusters);

void calculateDistance(const std::vector<unsigned int> &centers,
		std::list<Pair> &selected, const arma::mat &points);

std::list<Pair>::iterator choose(
		boost::random::bernoulli_distribution<> &bernoulli,
		boost::random::mt19937 &gen, std::list<Pair> &selected);

class KmeansppAssignment: public Assignment {
public:
	KmeansppAssignment(const arma::mat &points, const int nrOfClusters) :
			Assignment::Assignment(points, nrOfClusters) {
	}
	virtual void operator()(std::vector<unsigned int> &assignment);
};
}

#endif
