#include <stdlib.h>
#include <vector>
#include <RcppArmadillo.h>

#ifndef USER_ASSIGMENT_HPP
#define USER_ASSIGMENT_HPP

namespace gmum {

  void initAssignCentroids(std::vector<unsigned int> &assignment,
			const arma::mat &points,
			unsigned int nrOfClusters,
			const arma::mat &centroids);

}

#endif

