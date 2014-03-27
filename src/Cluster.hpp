#include <armadillo>
#include <vector>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

class Cluster {
private:
  int count;
  arma::rowvec mean;
  /*
   * covariance matrix 
   */
  arma::mat covMat;
  void initializeCovarianceMatrix(int id, std::vector<int> &fits, arma::mat &points);
public:
  Cluster();
  Cluster(int id, std::vector<int> &fits, arma::mat &points);
  Cluster addPoint(arma::rowvec &point);
  Cluster removePoint(arma::rowvec &point);
  float entropy();
  int size();
  arma::rowvec initializeMean(int id, std::vector<int> &fits, arma::mat &points);

  static int numberOfPoints;
};

#endif
