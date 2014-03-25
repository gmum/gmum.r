#include "Cluster.hpp"

Cluster::Cluster(int id, vector<int> &fits, NumericMatrix &points) {
  initializeMean(id, fits, points);
  initializeCovarianceMatrix(id, fits, points);
}

void Cluster::initializeMean(int id, vector<int> &fits, NumericMatrix &points) {
  int dimention = points.ncol();
  
  mean = NumericVector(1,dimention);
  std::fill(mean.begin(), mean.end(), 0);
  
  for(int i = 0; i < points.nrow(); i++) 
    if(fits[i] == id) mean += points(i,_);
  mean = mean/points.nrow();
}

void Cluster::initializeCovarianceMatrix(int id, vector<int> &fits, NumericMatrix &points) {
  int dimention = points.ncol();

  arma::rowvec m = as<arma::rowvec>(mean);

  arma::mat out(dimension, dimension, fill::zeros);
  for(int i = 0; i < points.nrow(); i++)
    if(fits[i] == id) {
      arma::rowvec point = as<arma::rowvec>(points(i,_));
      arma::rowvec tmp = point-m;
      out += tmp.t()*tmp;
    }

  covMat = as<NumericMatrix>(out);
}

Cluster Cluster::addPoint() {

}

Cluster Cluster::removePoint() {

}

float Cluster::entropy() {

}
