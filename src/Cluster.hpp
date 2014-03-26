#include <RcppArmadillo.h>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

class Cluster {
private:
  int count;
  Rcpp::NumericVector mean;
  /*
   * covariance matrix 
   */
  Rcpp::NumericMatrix covMat;
  static int numberOfPoints;
  void initializeMean(int id, std::vector<int> &fits, Rcpp::NumericMatrix &points);
  void initializeCovarianceMatrix(int id, std::vector<int> &fits, Rcpp::NumericMatrix &points);
public:
  Cluster(int id, std::vector<int> &fits, Rcpp::NumericMatrix &points);
  Cluster addPoint();
  Cluster removePoint();
  float entropy();
};

#endif
