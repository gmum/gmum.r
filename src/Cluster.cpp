#include "Cluster.hpp"

Cluster::Cluster(int id, vector<int> &fits, arma::mat &points) {
  initializeMean(id, fits, points);
  initializeCovarianceMatrix(id, fits, points);
}

void Cluster::initializeMean(int id, vector<int> &fits, arma::mat &points) {
  int dimention = points.ncol();
  
  mean = arma::rowvec(dimention, fill::zeros);
  
  for(int i = 0; i < points.n_rows; i++) 
    if(fits[i] == id) mean += points(i,_);
  mean = mean/points.n_rows;
}

void Cluster::initializeCovarianceMatrix(int id, vector<int> &fits, arma::mat &points) {
  int dimention = points.ncol();

  arma::rowvec m = as<arma::rowvec>(mean);

  arma::mat out(dimension, dimension, fill::zeros);
  for(int i = 0; i < points.n_rows; i++)
    if(fits[i] == id) {
      arma::rowvec point = points.row(i);
      arma::rowvec tmp = point-m;
      out += tmp.t()*tmp;
    }

  covMat = out;
}

Cluster Cluster::addPoint() {

}

Cluster Cluster::removePoint() {

}

float Cluster::entropy() {

}
