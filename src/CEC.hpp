#include "Cluster.hpp"
#include "Algorithm.hpp"
#include <armadillo>
#include <vector>
#include <boost/smart_ptr.hpp>

enum ClusterType {
  usual, covMatrix, constRadius, spherical, diagonal
};

class CEC {
private:

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
  int singleLoop();
  float entropy();
  std::vector<unsigned int> getAssignment();

  std::string test();
};
