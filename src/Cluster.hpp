#include <armadillo>
#include <vector>
#include <cmath>
#include <boost/smart_ptr.hpp>
#include "exceptions.hpp"

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
    double _entropy;

    virtual void calculateEntropy();
    void initializeCovarianceMatrix(unsigned int id,
				    const std::vector<unsigned int> &assignment, 
				    const arma::mat &points);
  
    Cluster(int _count, const arma::rowvec & _mean, const arma::mat & covMat);
    virtual boost::shared_ptr<Cluster> createInstance(int _count, 
						      const arma::rowvec & _mean,
						      const arma::mat & covMat);
  public:
    Cluster();
    Cluster(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
    virtual boost::shared_ptr<Cluster> addPoint(const arma::rowvec &point);
    virtual boost::shared_ptr<Cluster> removePoint(const arma::rowvec &point);
    double entropy();
    int size();
    arma::rowvec initializeMean(unsigned int id, 
				const std::vector<unsigned int> &assignment, 
				const arma::mat &points);
    arma::rowvec getMean();
    virtual arma::mat getCovMat();
    double predict(const arma::rowvec);
    static unsigned int numberOfPoints;
    virtual void computeCovarianceMatrix(unsigned int id, 
					 const std::vector<unsigned int> &assignment, 
					 const arma::mat &points);
  };


  class ClusterOnlyTrace : public Cluster {
  protected:
    double covMatTrace;
    bool _computedCovMat;
    void computeCovMatTrace();
    void computeCovMatTrace(unsigned int id, 
			    const std::vector<unsigned int> &assignment,
			    const arma::mat &points);
    virtual boost::shared_ptr<ClusterOnlyTrace>
    createInstance(int _count, const arma::rowvec & _mean, double _covMatTrace) = 0;
  public:
    virtual void computeCovarianceMatrix(unsigned int id, 
					 const std::vector<unsigned int> &assignment,
					 const arma::mat &points);
    ClusterOnlyTrace(int _count, const arma::rowvec & _mean, double _covMatTrace);
    ClusterOnlyTrace(unsigned int id, 
		     const std::vector<unsigned int> & assignment, 
		     const arma::mat & points);
    
    virtual boost::shared_ptr<Cluster> addPoint(const arma::rowvec &point);
    virtual boost::shared_ptr<Cluster> removePoint(const arma::rowvec &point);
    bool computedCovMat();
    // for test purposes
    double getCovMatTrace();
  };


  class ClusterCovMat : public Cluster {
    void calculateEntropy();
    arma::mat invSigma;
    double sigmaDet;
    boost::shared_ptr<Cluster> createInstance(int _count,
					      const arma::rowvec & _mean,
					      const arma::mat & covMat);
  public:
    ClusterCovMat(const arma::mat & invSigma, double sigmaDet, int _count,
		  const arma::rowvec & _mean, const arma::mat & covMat);
    ClusterCovMat(const arma::mat & sigma, unsigned int id,
		  const std::vector<unsigned int> &assignment,
		  const arma::mat &points);
  };

  class ClusterConstRadius : public ClusterOnlyTrace {
    void calculateEntropy();
    double r;
    boost::shared_ptr<ClusterOnlyTrace> createInstance(int _count, 
						       const arma::rowvec & _mean, 
						       double _covMatTrace);
  public:
    ClusterConstRadius(double r, int _count, const arma::rowvec & _mean, double _covMatTrace);
    ClusterConstRadius(double r, unsigned int id, 
		       const std::vector<unsigned int> &assignment, 
		       const arma::mat &points);
  };


  class ClusterSpherical : public ClusterOnlyTrace {
    void calculateEntropy();
    boost::shared_ptr<ClusterOnlyTrace> createInstance(int _count, 
						       const arma::rowvec & _mean, 
						       double _covMatTrace);
  public:
    ClusterSpherical(int _count,const arma::rowvec & _mean,
		     double _covMatTrace);
    ClusterSpherical(unsigned int id, 
		     const std::vector<unsigned int> &assignment, 
		     const arma::mat &points);
  };


  class ClusterDiagonal : public Cluster {
    void calculateEntropy();
    boost::shared_ptr<Cluster> createInstance(int _count,
					      const arma::rowvec & _mean,
					      const  arma::mat & covMat);
  public:
    ClusterDiagonal(int _count,const arma::rowvec & _mean, const arma::mat & covMat);
    ClusterDiagonal(unsigned int id, 
		    const std::vector<unsigned int> &assignment, 
		    const arma::mat &points);
  };

}

#endif
