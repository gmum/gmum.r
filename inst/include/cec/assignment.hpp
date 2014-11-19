#include <RcppArmadillo.h>
#include "boost/foreach.hpp"

#ifndef ASSIGMENT_HPP
#define ASSIGMENT_HPP

namespace gmum {

  /**
   * Different methods of initiating assignment inherit from this class.
   * You use operator() for getting new assignment.
   */
  class Assignment {
  protected:
    const arma::mat &points;
    const unsigned int nrOfClusters;
  public:
    Assignment(const arma::mat &points, const unsigned int nrOfClusters)
      : points(points), nrOfClusters(nrOfClusters) {};
    virtual void operator() (std::vector<unsigned int> &assignment) = 0;
    virtual ~Assignment() {}
  };

  /**
   * @centers are ids of rows in points
   */
  unsigned int findNearest(unsigned int i, 
			   const std::vector<unsigned int> &centers,
			   const arma::mat &points);
		   
  unsigned int findNearest(unsigned int i, 
			   const std::list<std::vector<double> > &centers,
			   const arma::mat &points);
	
	/**
   * @centers are ids of rows in points
   */		   
  void assignPoints(std::vector<unsigned int> &assignment,
		    const std::vector<unsigned int> &centers,
		    const arma::mat &points);

  void assignPoints(std::vector<unsigned int> &assignment,
		    const std::list<std::vector<double> > &centers,
		    const arma::mat &points);
}

#endif
