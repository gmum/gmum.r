#include <RcppArmadillo.h>
#include <vector>
#include <list>
#include <boost/smart_ptr.hpp>
#include "Cluster.hpp"
#include "Algorithm.hpp"
#include "exceptions.hpp"

#ifndef CEC_HPP
#define CEC_HPP

namespace gmum {

  enum ClusterType {
    standard, full, diagonal, sphere, fsphere
  };

  class CEC {
    
  private:
    TotalResult result;
    boost::shared_ptr<std::vector<unsigned int> > assignment;
    boost::shared_ptr<const arma::mat> points;
    boost::shared_ptr<Algorithm> algorithm;
    const double killThreshold;
  public:
    CEC(boost::shared_ptr<const arma::mat> points, 
	boost::shared_ptr<std::vector<unsigned int> > assignment, 
	boost::shared_ptr<Algorithm> algorithm,
	double killThreshold, const std::vector<ClusterType> &type,
	const std::vector<double> &radius, const std::vector<arma::mat> &covMatrices);

    CEC(boost::shared_ptr<const arma::mat> points, 
	boost::shared_ptr<std::vector<unsigned int> > assignment, 
	boost::shared_ptr<Algorithm> algorithm,
	double killThreshold, int numberOfClusters);

    // for debug purposes
    std::vector<boost::shared_ptr<Cluster> > clusters;
    void loop();
    void singleLoop();
    double entropy();
    std::vector<unsigned int> getAssignment();
    std::vector<arma::rowvec> centers();
    std::vector<arma::mat> cov();
    unsigned int iters();
    std::list<unsigned int> getNrOfClusters();
    boost::shared_ptr<const arma::mat> getPtrToPoints() const;
    boost::shared_ptr<std::vector<unsigned int> > getPtrToAssignement() const;
    std::list<double> getEnergy();
  };

}

#endif
