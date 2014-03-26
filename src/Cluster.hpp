#include <armadillo>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

class Cluster {
private:
  int count;
  arma::vec mean;
  /*
   * covariance matrix 
   */
  arma::mat covMat;
  static int numberOfPoints;
  void initializeMean(int id, std::vector<int> &fits, arma::mat &points);
  void initializeCovarianceMatrix(int id, std::vector<int> &fits, arma::mat &points);
public:
  Cluster(int id, std::vector<int> &fits, arma::mat &points);
  Cluster addPoint();
  Cluster removePoint();
  float entropy();
};

#endif
