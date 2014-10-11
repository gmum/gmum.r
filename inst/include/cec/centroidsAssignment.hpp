#include <stdlib.h>
#include <vector>
#include <RcppArmadillo.h>
#include "assignment.hpp"

#ifndef USER_ASSIGMENT_HPP
#define USER_ASSIGMENT_HPP

namespace gmum {

  /**
   * points are labeled by the closest centroid
   */
  class CentroidsAssignment : public Assignment {
  protected:
    const std::list<std::vector<double> > &centroids;
  public:
    CentroidsAssignment(const arma::mat &points,
			const std::list<std::vector<double> > &centr)
      : Assignment::Assignment(points, nrOfClusters), centroids(centr) {};
    virtual void operator() (std::vector<unsigned int> &assignment);
  };

}

#endif

