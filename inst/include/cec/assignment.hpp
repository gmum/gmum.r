#include <RcppArmadillo.h>

#ifndef ASSIGMENT_HPP
#define ASSIGMENT_HPP

namespace gmum {
	
  class Assignment {
  protected:
    const arma::mat &points;
    const int nrOfClusters;
  public:
    Assignment(const arma::mat &points, const int nrOfClusters)
      : points(points), nrOfClusters(nrOfClusters) {};
    virtual void operator() (std::vector<unsigned int> &assignment) = 0;
    virtual ~Assignment() {}
  };

  unsigned int findNearest(unsigned int i, 
			   const std::vector<unsigned int> &centers,
			   const arma::mat &points);

  void assignPoints(std::vector<unsigned int> &assignment,
		    const std::vector<unsigned int> &centers,
		    const arma::mat &points);
}

#endif
