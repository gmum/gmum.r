#ifndef CENTROIDASSIGNMENT_HPP
#define CENTROIDASSIGNMENT_HPP

#include <vector>
#include <stdlib.h>
#include "assignment.hpp"

namespace gmum {

/**
 * Points are labeled by the closest centroid
 */
class CentroidsAssignment: public Assignment {
protected:
	const std::list<std::vector<double> > &centroids;
public:
	CentroidsAssignment(const arma::mat &points,
			const std::list<std::vector<double> > &centr) :
			Assignment::Assignment(points, nrOfClusters), centroids(centr) {
	}
	virtual void operator()(std::vector<unsigned int> &assignment);
};

}

#endif

