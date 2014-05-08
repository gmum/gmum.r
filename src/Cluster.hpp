
#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <armadillo>
#include <vector>

class Cluster {
protected:
  unsigned int N;
  int count;
  arma::rowvec mean;
  /*
   * covariance matrix 
   */
  arma::mat covMat;
  float _entropy;

  virtual void calculateEntropy();
  void initializeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  
  Cluster(int _count,arma::rowvec & _mean, arma::mat & covMat);
public:
  Cluster();
  Cluster(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  Cluster addPoint(arma::rowvec &point);
  Cluster removePoint(arma::rowvec &point);
  float entropy();
  int size();
  arma::rowvec initializeMean(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  arma::rowvec getMean();
  arma::mat getCovMat();

  static unsigned int numberOfPoints;
};

class ClusterCovMat : public Cluster {
  void calculateEntropy();
  arma::mat sigma;
public:
  ClusterCovMat(arma::mat sigma, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
};

class ClusterConstRadius : public Cluster {
  void calculateEntropy();
  float r;
public:
  ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
};

class ClusterSpherical : public Cluster {
  void calculateEntropy();
public:
  ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
};

class ClusterDiagonal : public Cluster {
  void calculateEntropy();
public:
  ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
};
#endif
