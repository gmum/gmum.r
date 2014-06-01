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
    virtual boost::shared_ptr<Cluster> createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat);
  public:
    Cluster();
    Cluster(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
    virtual boost::shared_ptr<Cluster> addPoint(arma::rowvec &point);
    virtual boost::shared_ptr<Cluster> removePoint(arma::rowvec &point);
    float entropy();
    int size();
    arma::rowvec initializeMean(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
    arma::rowvec getMean();
    virtual arma::mat getCovMat();
    float predict(arma::rowvec);
    static unsigned int numberOfPoints;
  };

  class ClusterCovMat : public Cluster {
    void calculateEntropy();
    arma::mat invSigma;
    float sigmaDet;
    boost::shared_ptr<Cluster> createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat);
  public:
    ClusterCovMat(const arma::mat & invSigma, float sigmaDet, int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterCovMat(const arma::mat & sigma, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterConstRadius : public ClusterOnlyTrace {
    void calculateEntropy();
    float r;
    virtual boost::shared_ptr<Cluster> createInstance(int _count,arma::rowvec & _mean);
  public:
    ClusterConstRadius(float r, int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterConstRadius(float r, unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };


  class ClusterSpherical : public ClusterOnlyTrace {
    void calculateEntropy();
    boost::shared_ptr<Cluster> createInstance(int _count,arma::rowvec & _mean);
  public:
    ClusterSpherical(int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterSpherical(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterDiagonal : public Cluster {
    void calculateEntropy();
    virtual boost::shared_ptr<Cluster> createInstance(int _count,arma::rowvec & _mean, arma::mat & covMat);
  public:
    ClusterDiagonal(int _count,arma::rowvec & _mean, arma::mat & covMat);
    ClusterDiagonal(unsigned int id, std::vector<unsigned int> &assignment, arma::mat &points);
  };

  class ClusterOnlyTrace : public Cluster {
  protected:

    
    double covMatTrace;
    bool _computedCovMat;
    void computeCovMatTrace();
    void computeCovMatTrace(unsigned int id, std::vector<unsigned int> &assignment,const arma::mat &points);
    virtual boost::shared_ptr<ClusterOnlyTrace> createInstance(int _count,const arma::rowvec & _mean, float _covMatTrace);
  public:
    ClusterOnlyTrace(int _count,const arma::rowvec & _mean, float _covMatTrace);
    ClusterOnlyTrace(unsigned int id, std::vector<unsigned int> & assignment, arma::mat & points);
    
    virtual boost::shared_ptr<Cluster> addPoint(arma::rowvec &point);
    virtual boost::shared_ptr<Cluster> removePoint(arma::rowvec &point);
    bool computedCovMat();
    // for test purposes
    double getCovMatTrace();
  };



}

#endif
