#include "Cluster.hpp"
#include "Algorithm.hpp"
#include <armadillo>
#include <vector>
#include <list>
#include <boost/smart_ptr.hpp>

#ifndef CEC_HPP
#define CEC_HPP

enum ClusterType {
  standard, full, diagonal, spherical, fsphere
};

class CEC {
private:
  TotalResult result;
  boost::shared_ptr<std::vector<unsigned int> > assignment;
  boost::shared_ptr<arma::mat> points;
  boost::shared_ptr<Algorithm> algorithm;
  const float killThreshold;
public:
  CEC(boost::shared_ptr<arma::mat> points, 
      boost::shared_ptr<std::vector<unsigned int> > assignment, 
      boost::shared_ptr<Algorithm> algorithm,
      float killThreshold, std::vector<ClusterType> type,
      std::vector<float> radius, std::vector<arma::mat> covMatrices);

  CEC(boost::shared_ptr<arma::mat> points, 
      boost::shared_ptr<std::vector<unsigned int> > assignment, 
      boost::shared_ptr<Algorithm> algorithm,
      float killThreshold, int numberOfClusters);

  // for debug purposes
  std::vector<Cluster> clusters;
  void loop();
  void singleLoop();
  float entropy();
  std::vector<unsigned int> getAssignment();
  std::vector<arma::rowvec> centers();
  std::vector<arma::mat> cov();
  unsigned int iters();
  std::list<unsigned int> getNrOfClusters();
  std::list<float> getEnergy();
};

#endif
