#include "assignment.hpp"

namespace gmum {

  unsigned int findNearest(unsigned int i, 
			   const std::vector<unsigned int> &centers,
			   const arma::mat &points) {

    arma::rowvec point = points.row(i);
    float distance = std::numeric_limits<float>::max();
    unsigned int nearest = -1;

    for(unsigned int i=0; i<centers.size(); ++i) {

      arma::rowvec vec = points.row(centers[i]) - point;
      float tempDist = arma::as_scalar(vec*vec.t());

      if(distance > tempDist) {
	distance = tempDist;
	nearest = i;
      }
      
    }
    return nearest;
  }

  void assignPoints(std::vector<unsigned int> &assignment,
		    const std::vector<unsigned int> &centers,
		    const arma::mat &points) {

    for(unsigned int i=0; i<assignment.size(); ++i)
      assignment[i] = findNearest(i, centers, points);
  }

}
