#include "Cluster.hpp"
#include <cmath>

Cluster::Cluster(int _count, arma::rowvec & _mean, arma::mat & _covMat):
  count(_count),mean(_mean),covMat(_covMat) {
  N = covMat.n_cols;
  calculateEntropy();

}

Cluster::Cluster() {}

Cluster::Cluster(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points) {
  initializeMean(id, assignment, points);
  initializeCovarianceMatrix(id, assignment, points);
    N = points.n_cols;
  calculateEntropy();

}

arma::rowvec Cluster::initializeMean(unsigned int id, std::vector<unsigned int> &assignment,
				     arma::mat &points) {
  int dimention = points.n_cols;
  
  count = 0;
  mean = arma::rowvec(dimention, arma::fill::zeros);
  
  for(unsigned int i = 0; i < points.n_rows; i++) {
    if(assignment[i] == id) {
      mean += points.row(i);
      count +=1;
    }
  }
  mean = mean/count;

  return mean;
}

void Cluster::initializeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &assignment,
					 arma::mat &points) {
  int dimension = points.n_cols;

  arma::rowvec m = mean;

  arma::mat out(dimension, dimension, arma::fill::zeros);
  for(unsigned int i = 0; i < points.n_rows; i++)
    if(assignment[i] == id) {
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

int Cluster::size() {
  return count;
}

arma::rowvec Cluster::getMean(){
  return mean;
}

arma::mat Cluster::getCovMat() {
  return covMat;
}

float Cluster::entropy() {
  return _entropy;
}

unsigned int Cluster::numberOfPoints = 0;

/*
 * entropy
 */

void Cluster::calculateEntropy() {
  float p = 1.0*count / numberOfPoints;
  _entropy = N*log(2*M_PI*M_E)/2 + log(arma::det(covMat))/2 + p*((-log(p)));
}

ClusterCovMat::ClusterCovMat(arma::mat sigma, unsigned int id, std::vector<unsigned int> &assignment,
			     arma::mat &points) : Cluster(id,assignment,points), sigma(sigma){}

void ClusterCovMat::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
  _entropy = N*log(2*M_PI)/2 + arma::trace(arma::inv(sigma)*covMat)/2 + N*log(arma::det(sigma))/2+  p*(-log(p));
}

ClusterConstRadius::ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment,
				       arma::mat &points) : Cluster(id,assignment,points), r(r) {}

void ClusterConstRadius::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = N*log(2*M_PI)/2 + arma::trace(covMat)/(2*r) + N*log(r)/2 + p * (-log(p));
}

ClusterSpherical::ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment,
				   arma::mat &points) : Cluster(id,assignment,points) {}

void ClusterSpherical::calculateEntropy() {
      float p = 1.0*count / numberOfPoints;
  _entropy = N*log(2*M_PI*M_E/N)/2 + N*log(arma::trace(covMat))/2 + p*(-log(p));
}

ClusterDiagonal::ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment,
				 arma::mat &points) : Cluster(id,assignment,points) {}

void ClusterDiagonal::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = N*log(2*M_PI*M_E)/2 + log(arma::det(arma::diagmat(covMat)))/2 + p*(-log(p));
}
