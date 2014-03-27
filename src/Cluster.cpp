#include "Cluster.hpp"

Cluster::Cluster() {}

Cluster::Cluster(int id, std::vector<int> &fits, arma::mat &points) {
  initializeMean(id, fits, points);
  initializeCovarianceMatrix(id, fits, points);
}

arma::rowvec Cluster::initializeMean(int id, std::vector<int> &fits, arma::mat &points) {
  int dimention = points.n_cols;
  
  mean = arma::rowvec(dimention, arma::fill::zeros);
  
  for(unsigned int i = 0; i < points.n_rows; i++) 
    if(fits[i] == id) mean += points.row(i);
  mean = mean/points.n_rows;
  return mean;
}

void Cluster::initializeCovarianceMatrix(int id, std::vector<int> &fits, arma::mat &points) {
  int dimension = points.n_cols;

  arma::rowvec m = mean;

  arma::mat out(dimension, dimension, arma::fill::zeros);
  for(unsigned int i = 0; i < points.n_rows; i++)
    if(fits[i] == id) {
      arma::rowvec point = points.row(i);
      arma::rowvec tmp = point-m;
      out += tmp.t()*tmp;
    }

  covMat = out;
}

Cluster Cluster::addPoint(arma::rowvec &point) {
  return *this;
}

Cluster Cluster::removePoint(arma::rowvec &point) {
  return *this;
}

float Cluster::entropy() {
  return 0;
}

int Cluster::size() {
  return count;
}
