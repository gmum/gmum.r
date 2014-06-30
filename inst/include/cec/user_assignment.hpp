#include <stdlib.h>
#include <vector>
#include <RcppArmadillo.h>
#include "assignment.hpp"

#ifndef USER_ASSIGMENT_HPP
#define USER_ASSIGMENT_HPP

namespace gmum {

  void initAssignCentroids(std::vector<unsigned int> &assignment,
			const arma::mat &points,
			unsigned int nrOfClusters,
			const std::list<Rcpp::List> &centroids);

class CentroidsAssignment : public Assignment {
protected:
  const std::list<Rcpp::List> &centroids;
public:
  CentroidsAssignment(const std::list<Rcpp::List> &centr) : 
   Assignment::Assignment(points, nrOfClusters), centroids(centr) {};
  virtual void operator() (std::vector<unsigned int> &assignment){
				initAssignCentroids(assignment, points, nrOfClusters, centroids);
			}
};

}

#endif

