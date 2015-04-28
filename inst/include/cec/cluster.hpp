#ifndef CLUSTER_HPP
#define CLUSTER_HPP

//add_point and remove_point are updating the cluster. nice! madry stan

#include <armadillo>
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
	unsigned int m_n;
	int m_count;
	arma::rowvec m_mean;
	double m_entropy;

	Cluster(int count, const arma::rowvec& mean);
	Cluster(unsigned int id, const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	arma::rowvec initialize_mean(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
public:
    virtual ~Cluster() { }
	virtual void add_point(const arma::rowvec& point) = 0;
	virtual void remove_point(const arma::rowvec& point) = 0;

	virtual double entropy_after_add_point(const arma::rowvec &point) = 0;
	virtual double entropy_after_remove_point(const arma::rowvec &point) = 0;
	virtual Cluster* clone() = 0;

	double entropy() const;
	int size() const;
	arma::rowvec get_mean();
	virtual arma::mat get_cov_mat(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points) = 0;
};

//abstract, never created
class ClusterUseCovMat: public Cluster {
protected:
	arma::mat m_cov_mat;
	arma::mat m_cov_mat_tmp;

	void initialize_cov_mat(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	ClusterUseCovMat(int count, const arma::rowvec &mean,
			const arma::mat& cov_mat);
	ClusterUseCovMat(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);

	virtual double calculate_entropy(int n, const arma::mat &cov_mat) = 0;
public:
	void add_point(const arma::rowvec &point);
	void remove_point(const arma::rowvec &point);

	double entropy_after_add_point(const arma::rowvec &point);
	double entropy_after_remove_point(const arma::rowvec &point);

	virtual arma::mat get_cov_mat(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
    virtual ~ClusterUseCovMat() { }
};

//abstract, never created
class ClusterOnlyTrace: public Cluster {
protected:
	double m_cov_mat_trace;
	ClusterOnlyTrace(int count, const arma::rowvec & mean,
			double cov_mat_trace);
	ClusterOnlyTrace(unsigned int id,
			const std::vector<unsigned int> & assignment,
			const arma::mat & points);
	void compute_cov_mat_trace(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual double calculate_entropy(double, int) =0;
public:
	virtual arma::mat get_cov_mat(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);

	void add_point(const arma::rowvec & point);
	void remove_point(const arma::rowvec &point);

	double entropy_after_add_point(const arma::rowvec &point);
	double entropy_after_remove_point(const arma::rowvec &point);

	double get_cov_mat_trace();

    virtual ~ClusterOnlyTrace() { }

};

class ClusterStandard: public ClusterUseCovMat {
private:
	double calculate_entropy(int n, const arma::mat &cov_mat);

public:
	ClusterStandard(int count, const arma::rowvec &mean,
			const arma::mat &cov_mat);
	ClusterStandard(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual ClusterStandard* clone();

    virtual ~ClusterStandard() { }

    virtual arma::mat get_cov_mat(unsigned int id,
                                  const std::vector<unsigned int> &assignment,
                                  const arma::mat &points);
};

class ClusterCovariance : public ClusterUseCovMat {
private:
	arma::mat m_inv_sigma;
	double m_sigma_det;

	double calculate_entropy(int n, const arma::mat &cov_mat);
public:
	ClusterCovariance(const arma::mat& inv_sigma, double sigma_det, int count,
			const arma::rowvec & mean, const arma::mat & cov_mat);
	ClusterCovariance(const arma::mat & sigma, unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual ClusterCovariance * clone();
    virtual ~ClusterCovariance() { }

    virtual arma::mat get_cov_mat(unsigned int id,
                                  const std::vector<unsigned int> &assignment,
                                  const arma::mat &points);
};

class ClusterSphericalFixedR : public ClusterOnlyTrace {
private:
	double calculate_entropy(double, int);
	double m_r;
public:
	ClusterSphericalFixedR(double r, int count, const arma::rowvec & mean,
			double cov_mat_trace);
	ClusterSphericalFixedR(double r, unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual ClusterSphericalFixedR * clone();

    virtual ~ClusterSphericalFixedR() { }

    virtual arma::mat get_cov_mat(unsigned int id,
                                  const std::vector<unsigned int> &assignment,
                                  const arma::mat &points);
};

class ClusterSpherical: public ClusterOnlyTrace {
private:
	double calculate_entropy(double, int);
public:
	ClusterSpherical(int count, const arma::rowvec & mean,
			double cov_mat_trace);
	ClusterSpherical(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual ClusterSpherical* clone();

    virtual ~ClusterSpherical() { }

    virtual arma::mat get_cov_mat(unsigned int id,
                                  const std::vector<unsigned int> &assignment,
                                  const arma::mat &points);
};

class ClusterDiagonal: public ClusterUseCovMat {
private:
	double calculate_entropy(int n, const arma::mat &cov_mat);
public:
	ClusterDiagonal(int count, const arma::rowvec & mean,
			const arma::mat & cov_mat);
	ClusterDiagonal(unsigned int id,
			const std::vector<unsigned int> &assignment,
			const arma::mat &points);
	virtual ClusterDiagonal* clone();

    virtual ~ClusterDiagonal() { }

    virtual arma::mat get_cov_mat(unsigned int id,
                                  const std::vector<unsigned int> &assignment,
                                  const arma::mat &points);
};

}

#endif
