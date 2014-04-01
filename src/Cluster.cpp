#include "Cluster.hpp"

Cluster::Cluster(int _count, arma::rowvec & _mean, arma::mat & _covMat):
  count(_count),mean(_mean),covMat(_covMat){}

Cluster::Cluster() {}

Cluster::Cluster(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points) {
  initializeMean(id, fits, points);
  initializeCovarianceMatrix(id, fits, points);
}

arma::rowvec Cluster::initializeMean(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points) {
  int dimention = points.n_cols;
  
  count = 0;
  mean = arma::rowvec(dimention, arma::fill::zeros);
  
  for(unsigned int i = 0; i < points.n_rows; i++) {
    if(fits[i] == id) {
      mean += points.row(i);
      count +=1;
    }
  }
  mean = mean/count;

  return mean;
}

void Cluster::initializeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &fits, arma::mat &points) {
  int dimension = points.n_cols;

  arma::rowvec m = mean;

  arma::mat out(dimension, dimension, arma::fill::zeros);
  for(unsigned int i = 0; i < points.n_rows; i++)
    if(fits[i] == id) {
      arma::rowvec point = points.row(i);
      arma::rowvec tmp = point-m;
      out += (tmp.t()*tmp)/(count);
    }

  covMat = out;
}

Cluster Cluster::addPoint(arma::rowvec &point) {
  int ncount = count+1;
  arma::rowvec nmean =  (count*mean + point)/(ncount);
  arma::rowvec  r = mean - point;
  arma::mat nCovMat = (1.0*count/ncount)*(covMat +(r.t() * r)/ncount);
  return Cluster(ncount,nmean,nCovMat);
}

Cluster Cluster::removePoint(arma::rowvec &point) {
  int ncount = count -1;
  arma::rowvec nmean = (count*mean - point)/ncount;
  arma::rowvec r = mean -point;
  arma::mat nCovMat = (1.0*count/ncount)*(covMat - (r.t()*r)/ncount);
  return Cluster(ncount,nmean,nCovMat);
}

float Cluster::entropy() {
  return 0;
}

int Cluster::size() {
  return count;
}

arma::rowvec Cluster::getMean(){
  return mean;
}

arma::mat Cluster::getCovMat() {
  return covMat;
}

unsigned int Cluster::numberOfPoints = 0;
