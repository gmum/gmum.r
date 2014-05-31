#include "Cluster.hpp"

namespace gmum {

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

  boost::shared_ptr<Cluster> Cluster::addPoint(arma::rowvec &point) {
    int ncount = count+1;
    arma::rowvec nmean =  (count*mean + point)/(ncount);
    arma::rowvec  r = mean - point;
    arma::mat nCovMat = (1.0*count/ncount)*(covMat +(r.t() * r)/ncount);
    //    return Cluster(ncount,nmean,nCovMat);
    return createInstance(ncount,nmean,nCovMat);
  }

  boost::shared_ptr<Cluster> Cluster::removePoint(arma::rowvec &point) {
    int ncount = count -1;
    arma::rowvec nmean = (count*mean - point)/ncount;
    arma::rowvec r = mean -point;
    arma::mat nCovMat = (1.0*count/ncount)*(covMat - (r.t()*r)/ncount);
    //    return Cluster(ncount,nmean,nCovMat);
    return createInstance(ncount,nmean,nCovMat);
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

  float Cluster::predict(arma::rowvec x) {
    float constMultiplier = sqrt(1.0/(pow(2*M_PI, x.n_cols)*arma::det(covMat)));
    float scalar = arma::as_scalar((x-mean)*arma::inv(covMat)*((x-mean).t()));
    float exponens = exp(-0.5*scalar);

    return constMultiplier*exponens;
  }

  unsigned int Cluster::numberOfPoints = 0;

  /*
   * entropy
   */

  void Cluster::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p* (N*log(2*M_PI*M_E)/2 + log(arma::det(covMat))/2 + (-log(p)));
  }

  ClusterCovMat::ClusterCovMat(arma::mat sigma, unsigned int id, std::vector<unsigned int> &assignment,
			       arma::mat &points) : Cluster(id,assignment,points) {
    sigmaDet = arma::det(sigma);
    invSigma = arma::inv(sigma);
  }

  void ClusterCovMat::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy =p*( N*log(2*M_PI)/2 + arma::trace(invSigma*covMat)/2 + N*log(sigmaDet)/2 -log(p));
  }

  ClusterConstRadius::ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment,
					 arma::mat &points) : Cluster(id,assignment,points), r(r) {}

  void ClusterConstRadius::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*(N*log(2*M_PI)/2 + arma::trace(covMat)/(2*r) + N*log(r)/2 -log(p));
  }

  ClusterSpherical::ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points) : Cluster() {
    covMatComputed = false;
    initializeMean(id,assignment,points);
    
}

  void ClusterSpherical::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*(N*log(2*M_PI*M_E/N)/2 + N*log(arma::trace(covMat))/2 -log(p));
  }

  ClusterDiagonal::ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment,
				   arma::mat &points) : Cluster(id,assignment,points) {}

  void ClusterDiagonal::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*( N*log(2*M_PI*M_E)/2 + log(arma::det(arma::diagmat(covMat)))/2 -log(p));
  }
  /*
    New contructors
   */
  
  ClusterCovMat::ClusterCovMat(int _count,arma::rowvec & _mean, arma::mat & covMat) :
    Cluster(_count,_mean,covMat){}
  ClusterConstRadius::ClusterConstRadius(int _count,arma::rowvec & _mean, arma::mat & covMat) :
    Cluster(_count,_mean,covMat){}
  ClusterSpherical::ClusterSpherical(int _count,arma::rowvec & _mean, arma::mat & covMat) :
    Cluster(_count,_mean,covMat){}

  ClusterDiagonal::ClusterDiagonal(int _count,arma::rowvec & _mean, arma::mat & covMat) :
    Cluster(_count,_mean,covMat){}
  /*
   * CreateInstance 

   */
  boost::shared_ptr<Cluster> Cluster::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
    return boost::shared_ptr<Cluster>(new Cluster(_count,_mean,covMat));
  }
   
  boost::shared_ptr<Cluster> ClusterCovMat::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
      return boost::shared_ptr<Cluster>(new ClusterCovMat(_count,_mean,covMat));  
}

  boost::shared_ptr<Cluster> ClusterConstRadius::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
      return boost::shared_ptr<Cluster>(new ClusterConstRadius(_count,_mean,covMat));
  }

  boost::shared_ptr<Cluster> ClusterSpherical::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
      return boost::shared_ptr<Cluster>(new ClusterSpherical(_count,_mean,covMat));
  }

  boost::shared_ptr<Cluster> ClusterDiagonal::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
      return boost::shared_ptr<Cluster>(new ClusterDiagonal(_count,_mean,covMat));
  }
   
}
