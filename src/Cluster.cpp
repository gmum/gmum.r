#include "Cluster.hpp"

namespace gmum {

  Cluster::Cluster(int _count, arma::rowvec & _mean, arma::mat & _covMat):
    count(_count),mean(_mean),covMat(_covMat) {
    N = covMat.n_cols;
    calculateEntropy();

  }

  Cluster::Cluster() {}

  Cluster::Cluster(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points) {
    try {
      initializeMean(id, assignment, points);
    } catch(std::exception e) {
      std::cout << "mean" << std::endl;
      throw(e);
    }

    if(count == 0) throw(NoPointsInCluster());

    try {
      initializeCovarianceMatrix(id, assignment, points);
    } catch(std::exception e) {
      std::cout << "covariance" << std::endl;
      throw(e);
    }

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
    arma::rowvec r = mean - point;
    arma::mat nCovMat = (1.0*count/ncount)*(covMat +(r.t() * r)/ncount);

    return createInstance(ncount,nmean,nCovMat);
  }

  boost::shared_ptr<Cluster> Cluster::removePoint(arma::rowvec &point) {
    int ncount = count-1;
    arma::rowvec nmean = (count*mean - point)/ncount;
    arma::rowvec r = mean - point;
    arma::mat nCovMat = (1.0*count/ncount)*(covMat - (r.t()*r)/ncount);

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

  void Cluster::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    try {
      _entropy = p* (N*log(2*M_PI*M_E)/2 + log(arma::det(covMat))/2 + (-log(p)));
    } catch(std::exception e) {
      std::cout << "calculate" << std::endl;
      std::cout << count << std::endl;
      std::cout << covMat << std::endl;
      std::cout << arma::det(covMat) << std::endl;
      throw(e);
    }
  }

  void Cluster::computeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points) {}



  ClusterOnlyTrace::ClusterOnlyTrace(int _count, const arma::rowvec & _mean, double _covMatTrace) {
    covMatTrace = _covMatTrace;
    count = _count;
    mean = _mean;
    N = mean.n_cols;
    _computedCovMat = false;
  }

  ClusterOnlyTrace::ClusterOnlyTrace(unsigned int id, std::vector<unsigned int> & assignment, arma::mat & points) {
    initializeMean(id,assignment,points);
    N = points.n_cols;
    computeCovMatTrace(id,assignment,points);
    _computedCovMat = false;
  }

  void ClusterOnlyTrace::computeCovMatTrace(unsigned int id, std::vector<unsigned int> &assignment,const arma::mat &points) {
      
    covMatTrace = 0.0;
    for(unsigned int i = 0; i < points.n_rows; i++)
      if(assignment[i] == id) {
	arma::rowvec point = points.row(i);
	arma::rowvec tmp = point-mean;
	//covMatTrace += (dot(tmp,tmp))/(count);
	covMatTrace += dot(tmp,tmp);
      }
    covMatTrace /= count;
  }

  boost::shared_ptr<Cluster> ClusterOnlyTrace::addPoint(arma::rowvec & point) {
    int ncount = count+1;
    arma::rowvec nmean =  (count*mean + point)/(ncount);
    arma::rowvec r = nmean - point;
    arma::rowvec meanDiff = mean - nmean;
    double ntrace = ((covMatTrace + dot(meanDiff,meanDiff))*count  + dot(r,r))/ncount;
    return createInstance(ncount,nmean,ntrace);
  }

  boost::shared_ptr<Cluster> ClusterOnlyTrace::removePoint(arma::rowvec & point) {
    int ncount = count -1;
    arma::rowvec nmean = (count*mean - point)/(ncount);
    arma::rowvec meanDiff = mean - nmean;
    arma::rowvec r = nmean - point;
    double ntrace = ((covMatTrace + dot(meanDiff,meanDiff))*count  - dot(r,r))/ncount;
    return createInstance(ncount,nmean,ntrace);
  }
  
  bool ClusterOnlyTrace::computedCovMat() {
    return _computedCovMat;
  }

  double ClusterOnlyTrace::getCovMatTrace(){ 
    return covMatTrace;
  }

  void ClusterOnlyTrace::computeCovarianceMatrix(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points) {
    initializeCovarianceMatrix(id,assignment,points);
    _computedCovMat = true;
  }



  ClusterCovMat::ClusterCovMat(const arma::mat & sigma, unsigned int id, std::vector<unsigned int> &assignment,
			       arma::mat &points) : Cluster(id,assignment,points) {
    sigmaDet = arma::det(sigma);
    invSigma = arma::inv(sigma);
  }

  ClusterCovMat::ClusterCovMat(const arma::mat & _invSigma, float _sigmaDet,
			       int _count,arma::rowvec & _mean, arma::mat & covMat)
    : Cluster(_count,_mean,covMat), invSigma(_invSigma), sigmaDet(_sigmaDet){}

  void ClusterCovMat::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy =p*( N*log(2*M_PI)/2 + arma::trace(invSigma*covMat)/2 + N*log(sigmaDet)/2 -log(p));
  }



  ClusterConstRadius::ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment,
					 arma::mat &points) : ClusterOnlyTrace(id,assignment,points), r(r) {
    calculateEntropy();
  }

  ClusterConstRadius::ClusterConstRadius(float r, int _count, const arma::rowvec & _mean, double _covMatTrace) :
    ClusterOnlyTrace(_count,_mean,_covMatTrace), r(r){ 
    calculateEntropy();
  }

  void ClusterConstRadius::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*(N*log(2*M_PI)/2 + covMatTrace/(2*r) + N*log(r)/2 -log(p));
  }



  ClusterSpherical::ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points)
    : ClusterOnlyTrace(id,assignment,points) {
    calculateEntropy();
  }

  ClusterSpherical::ClusterSpherical(int _count, const arma::rowvec & _mean, double  _covMatTrace) :
    ClusterOnlyTrace(_count,_mean,_covMatTrace){
    calculateEntropy();
  }

  void ClusterSpherical::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*(N*log(2*M_PI*M_E/N)/2 + N*log(covMatTrace)/2 -log(p));
  }



  ClusterDiagonal::ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment,
				   arma::mat &points) : Cluster(id,assignment,points) {}

  ClusterDiagonal::ClusterDiagonal(int _count,arma::rowvec & _mean, arma::mat & covMat)
    : Cluster(_count,_mean,covMat){}

  void ClusterDiagonal::calculateEntropy() {
    float p = 1.0*count / numberOfPoints;
    _entropy = p*( N*log(2*M_PI*M_E)/2 + log(arma::det(arma::diagmat(covMat)))/2 -log(p));
  }



  boost::shared_ptr<Cluster> Cluster::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
    return boost::shared_ptr<Cluster>(new Cluster(_count,_mean,covMat));
  }
   
  boost::shared_ptr<Cluster> ClusterCovMat::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
    return boost::shared_ptr<Cluster>(new ClusterCovMat(invSigma,sigmaDet,_count,_mean,covMat));  
  }

  boost::shared_ptr<ClusterOnlyTrace> ClusterConstRadius::createInstance(int _count,const arma::rowvec & _mean, double _covMatTrace) {
    return boost::shared_ptr<ClusterOnlyTrace>(new ClusterConstRadius(r,_count,_mean,_covMatTrace));
  }

  boost::shared_ptr<ClusterOnlyTrace> ClusterSpherical::createInstance(int _count,const arma::rowvec & _mean, double _covMatTrace) {
    return boost::shared_ptr<ClusterOnlyTrace>(new ClusterSpherical(_count,_mean,_covMatTrace));
  }

  boost::shared_ptr<Cluster> ClusterDiagonal::createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat) {
    return boost::shared_ptr<Cluster>(new ClusterDiagonal(_count,_mean,covMat));
  }

}
