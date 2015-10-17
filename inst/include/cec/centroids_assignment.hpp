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
	const std::list<std::vector<double> > &m_centroids;
public:
	CentroidsAssignment(const arma::mat &points, const unsigned int nclusters,
			const std::list<std::vector<double> > &centr) :
			Assignment::Assignment(points, nclusters), m_centroids(centr) {
	}
	virtual void operator()(std::vector<unsigned int> &assignment);
};

}

#endif

