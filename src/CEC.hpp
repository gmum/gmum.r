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
    boost::shared_ptr<arma::mat> points;
    boost::shared_ptr<Algorithm> algorithm;
    const double killThreshold;
  public:
    CEC(boost::shared_ptr<arma::mat> points, 
	boost::shared_ptr<std::vector<unsigned int> > assignment, 
	boost::shared_ptr<Algorithm> algorithm,
	double killThreshold, std::vector<ClusterType> type,
	std::vector<double> radius, std::vector<arma::mat> covMatrices);

    CEC(boost::shared_ptr<arma::mat> points, 
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
    std::list<double> getEnergy();
  };

}

#endif
