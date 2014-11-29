#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <RcppArmadillo.h>
#include <cmath>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "exceptions.hpp"

namespace gmum {

/**
 * Cluster stores its entropy and knows how to update it
 */
class Cluster {
protected:
	unsigned int N;
	int count;
	arma::rowvec mean;
	double _entropy;

	virtual void calculateEntropy() = 0;
	Cluster(int, const arma::rowvec &);
	Cluster(unsigned int, const std::vector<unsigned int> &, const arma::mat &);
	arma::rowvec initializeMean(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
public:
	virtual ~Cluster() { }
	virtual boost::shared_ptr<Cluster> addPoint(const arma::rowvec &) = 0;
	virtual boost::shared_ptr<Cluster> removePoint(const arma::rowvec &) = 0;
	double entropy() const;
	int size() const;
	arma::rowvec getMean();
	virtual arma::mat getCovMat(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &) = 0;
};

class ClusterUseCovMat: public Cluster {
protected:
	arma::mat covMat;
	void initializeCovarianceMatrix(unsigned int,
			const std::vector<unsigned int>&, const arma::mat&);
	ClusterUseCovMat(int, const arma::rowvec&, const arma::mat&);
	ClusterUseCovMat(unsigned int, const std::vector<unsigned int>&,
			const arma::mat&);
	virtual boost::shared_ptr<ClusterUseCovMat> createInstance(int,
			const arma::rowvec &, const arma::mat &) = 0;
public:
	boost::shared_ptr<Cluster> addPoint(const arma::rowvec &);
	boost::shared_ptr<Cluster> removePoint(const arma::rowvec &);
	arma::mat getCovMat(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
};

class ClusterOnlyTrace: public Cluster {
protected:
	double covMatTrace;
	virtual boost::shared_ptr<ClusterOnlyTrace>
	createInstance(int, const arma::rowvec &, double) = 0;
	ClusterOnlyTrace(int, const arma::rowvec &, double);
	ClusterOnlyTrace(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
	void computeCovMatTrace(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
public:
	virtual arma::mat getCovMat(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
	virtual boost::shared_ptr<Cluster> addPoint(const arma::rowvec &);
	virtual boost::shared_ptr<Cluster> removePoint(const arma::rowvec &);
	double getCovMatTrace();
};

class ClusterStandard: public ClusterUseCovMat {
	void calculateEntropy();
	boost::shared_ptr<ClusterUseCovMat> createInstance(int,
			const arma::rowvec &, const arma::mat &);
public:
	ClusterStandard(int, const arma::rowvec &, const arma::mat &);
	ClusterStandard(unsigned int, const std::vector<unsigned int> &,
			const arma::mat &);
};

class ClusterCovMat: public ClusterUseCovMat {
	void calculateEntropy();
	arma::mat invSigma;
	double sigmaDet;
	boost::shared_ptr<ClusterUseCovMat> createInstance(int,
			const arma::rowvec &, const arma::mat &);
public:
	ClusterCovMat(const arma::mat &, double, int, const arma::rowvec &,
			const arma::mat &);
	ClusterCovMat(const arma::mat &, unsigned int,
			const std::vector<unsigned int> &, const arma::mat &);
};

class ClusterConstRadius: public ClusterOnlyTrace {
	void calculateEntropy();
	double r;
	boost::shared_ptr<ClusterOnlyTrace> createInstance(int,
			const arma::rowvec &, double);
public:
	ClusterConstRadius(double, int, const arma::rowvec &, double);
	ClusterConstRadius(double r, unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
};

class ClusterSpherical: public ClusterOnlyTrace {
	void calculateEntropy();
	boost::shared_ptr<ClusterOnlyTrace> createInstance(int _count,
			const arma::rowvec & _mean, double _covMatTrace);
public:
	ClusterSpherical(int _count, const arma::rowvec & _mean,
			double _covMatTrace);
	ClusterSpherical(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
};

class ClusterDiagonal: public ClusterUseCovMat {
	void calculateEntropy();
	boost::shared_ptr<ClusterUseCovMat> createInstance(int _count,
			const arma::rowvec & _mean, const arma::mat & covMat);
public:
	ClusterDiagonal(int _count, const arma::rowvec & _mean,
			const arma::mat & covMat);
	ClusterDiagonal(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
};

}

#endif
