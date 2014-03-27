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
  void initializeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
public:
  Cluster();
  Cluster(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  Cluster addPoint(arma::rowvec &point);
  Cluster removePoint(arma::rowvec &point);
  float entropy();
  int size();
  arma::rowvec initializeMean(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);

  static int numberOfPoints;
};

#endif
