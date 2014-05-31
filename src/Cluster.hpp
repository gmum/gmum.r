#include <armadillo>
#include <vector>
#include <cmath>
#include <boost/smart_ptr.hpp>

#ifndef CLUSTER_HPP
#define CLUSTER_HPP

namespace gmum {

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
    template<class T> boost::shared_ptr<Cluster>
    createInstance(T *instance, int _count,arma::rowvec & _mean, arma::mat & covMat);
  public:
    Cluster();
    Cluster(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
    boost::shared_ptr<Cluster> addPoint(arma::rowvec &point);
    boost::shared_ptr<Cluster> removePoint(arma::rowvec &point);
    float entropy();
    int size();
    arma::rowvec initializeMean(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
    arma::rowvec getMean();
    arma::mat getCovMat();
    float predict(arma::rowvec);
    static unsigned int numberOfPoints;
  };

  class ClusterCovMat : public Cluster {
    void calculateEntropy();
    arma::mat invSigma;
    float sigmaDet;
  public:
    ClusterCovMat(const arma::mat & invSigma, float sigmaDet, int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterCovMat(const arma::mat & sigma, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterConstRadius : public Cluster {
    void calculateEntropy();
    float r;
  public:
    ClusterConstRadius(float r, int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterSpherical : public Cluster {
    void calculateEntropy();
  public:
    ClusterSpherical(int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterDiagonal : public Cluster {
    void calculateEntropy();
  public:
    ClusterDiagonal(int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterOnlyTrace : public Cluster {
  protected:
    virtual void calculateEntropy() = 0;
    float covMatTrace;
    bool computedCovMat;
    void computeCovMatTrace(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  public:
    
  };

}

#endif
