#include <armadillo>
#include <vector>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

class Cluster {
protected:
  int count;
  arma::rowvec mean;
  /*
   * covariance matrix 
   */
  arma::mat covMat;
  void initializeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  
  Cluster(int _count,arma::rowvec & _mean, arma::mat & covMat);
public:
  Cluster();
  Cluster(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  Cluster addPoint(arma::rowvec &point);
  Cluster removePoint(arma::rowvec &point);
  virtual float entropy();
  int size();
  arma::rowvec initializeMean(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  arma::rowvec getMean();
  arma::mat getCovMat();

  static unsigned int numberOfPoints;
};

class ClusterCovMat : public Cluster {
  arma::mat sigma;
public:
  ClusterCovMat(arma::mat sigma, unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  float entropy();
};

class ClusterConstRadius : public Cluster {
  float r;
public:
  ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  float entropy();
};

class ClusterSpherical : public Cluster {
public:
  ClusterSpherical(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  float entropy();
};

class ClusterDiagonal : public Cluster {
public:
  ClusterDiagonal(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points);
  float entropy();
};
#endif
