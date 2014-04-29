#include "Cluster.hpp"
#include "Algorithm.hpp"
#include <armadillo>
#include <vector>

enum ClusterType {
  usual, covMatrix, constRadius, spherical, diagonal
};

class CEC {
private:
  std::vector<Cluster> clusters;
  std::vector<unsigned int> &assignment;
  arma::mat &points;
  Algorithm &algorithm;
  const float killThreshold;
public:
  CEC(arma::mat &points, std::vector<unsigned int> &assignment, 
      float killThreshold, Algorithm &algorithm,
      std::vector<ClusterType> &type, std::vector<float> &radius,
      std::vector<arma::mat> &covMatrices);

  CEC(arma::mat &points, std::vector<unsigned int> &assignment,
      float killThreshold, Algorithm &algorithm, int numberOfClusters);

  void loop();
  int singleLoop();
  float entropy();
};
