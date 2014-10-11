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
  
  unsigned int findNearest(unsigned int i, 
			   const std::list<std::vector<double> > &centers,
			   const arma::mat &points) {

    arma::rowvec point = points.row(i);
    float distance = std::numeric_limits<float>::max();
    unsigned int nearest = -1;
    unsigned int j = 0;
    
	BOOST_FOREACH(std::vector<double> center, centers) {
		
	  arma::rowvec centr = arma::conv_to<arma::rowvec>::from(center);
      arma::rowvec vec = centr - point;	
      float tempDist = arma::as_scalar(vec*vec.t());
      if(distance > tempDist) {
		distance = tempDist;
		nearest = j;
      }
      j++;
      
	}
	
    return nearest;
  }

  void assignPoints(std::vector<unsigned int> &assignment,
		    const std::vector<unsigned int> &centers,
		    const arma::mat &points) {

    for(unsigned int i=0; i<assignment.size(); ++i)
      assignment[i] = findNearest(i, centers, points);
  }
  
   void assignPoints(std::vector<unsigned int> &assignment,
		    const std::list<std::vector<double> > &centers,
		    const arma::mat &points) {

    for(unsigned int i=0; i<assignment.size(); ++i)
      assignment[i] = findNearest(i, centers, points);
  }

}
