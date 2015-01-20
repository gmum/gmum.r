#ifndef CLUSTER_HPP
#define CLUSTER_HPP

//add_point adn remove_point are updating the cluster. nice! madry stan

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

    virtual void calculate_entropy() = 0;
    Cluster(int count, const arma::rowvec& mean);
    Cluster(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
    arma::rowvec initialize_mean(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
public:
    virtual ~Cluster() { }
    virtual Cluster * add_point(const arma::rowvec& mean) = 0;
    virtual Cluster * remove_point(const arma::rowvec& mean) = 0;

    virtual double entropy_after_point_add(const arma::rowvec &point) const{
    	return 0;
    }
    virtual double entropy_after_point_remove(const arma::rowvec &) const{
    	return 0;
    }

    double entropy() const;
    int size() const;
    arma::rowvec get_mean();
    virtual arma::mat get_cov_mat(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points) = 0;
};

class ClusterUseCovMat: public Cluster {
protected:
    arma::mat m_cov_mat;

    void initialize_cov_mat(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
    ClusterUseCovMat(int count, const arma::rowvec &mean,const arma::mat& cov_mat);
    ClusterUseCovMat(unsigned int id,const std::vector<unsigned int> &assignment, const arma::mat &points);
    virtual ClusterUseCovMat * create_instance(int count, const arma::rowvec& mean, const arma::mat& cov_mat) = 0;
public:
    Cluster * add_point(const arma::rowvec &point);
    Cluster * remove_point(const arma::rowvec &point);



    arma::mat get_cov_mat(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
};

class ClusterOnlyTrace: public Cluster {
protected:
    double m_cov_mat_trace;
    virtual ClusterOnlyTrace * create_instance(int, const arma::rowvec& mean, double cov_mat_trace) = 0;
    ClusterOnlyTrace(int count, const arma::rowvec & mean, double cov_mat_trace);
    ClusterOnlyTrace(unsigned int id, const std::vector<unsigned int> & assignment, const arma::mat & points);
    void compute_cov_mat_trace(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
public:
    virtual arma::mat get_cov_mat(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
    virtual Cluster * add_point(const arma::rowvec & point);
    virtual Cluster * remove_point(const arma::rowvec &point);
    double get_cov_mat_trace();
};

class ClusterStandard: public ClusterUseCovMat {
private:
    void calculate_entropy();
    ClusterUseCovMat * create_instance(int count,const arma::rowvec &mean, const arma::mat &cov_mat);
public:
    ClusterStandard(int count, const arma::rowvec &mean, const arma::mat &cov_mat);
    ClusterStandard(unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);

//     double entropy_after_point_add(const arma::rowvec &point) const;
//     double entropy_after_point_remove(const arma::rowvec &) const;
};

class ClusterCovMat: public ClusterUseCovMat {
private:
    arma::mat m_inv_sigma;
    double m_sigma_det;

    ClusterUseCovMat * create_instance(int count, const arma::rowvec & mean, const arma::mat & cov_mat);
    void calculate_entropy();
public:
    ClusterCovMat(const arma::mat& inv_sigma, double sigma_det, int count, const arma::rowvec & mean, const arma::mat & cov_mat);
    ClusterCovMat(const arma::mat & sigma, unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
};

class ClusterConstRadius: public ClusterOnlyTrace {
private:
    void calculate_entropy();
    double m_r;
    ClusterOnlyTrace * create_instance(int count, const arma::rowvec & mean, double cov_mat_trace);
public:
    ClusterConstRadius(double r, int count,const arma::rowvec & mean, double cov_mat_trace);
    ClusterConstRadius(double r, unsigned int id, const std::vector<unsigned int> &assignment, const arma::mat &points);
};

class ClusterSpherical: public ClusterOnlyTrace {
private:
    void calculate_entropy();
    ClusterOnlyTrace * create_instance(int count,
                                                        const arma::rowvec &mean, double cov_mat_trace);
public:
    ClusterSpherical(int count, const arma::rowvec & mean,
                     double cov_mat_trace);
    ClusterSpherical(unsigned int id,
                     const std::vector<unsigned int> &assignment,
                     const arma::mat &points);
};

class ClusterDiagonal: public ClusterUseCovMat {
private:
    void calculate_entropy();
    ClusterUseCovMat * create_instance(int count, const arma::rowvec & mean, const arma::mat & cov_mat);
public:
    ClusterDiagonal(int count, const arma::rowvec & mean,
                    const arma::mat & cov_mat);
    ClusterDiagonal(unsigned int id,
                    const std::vector<unsigned int> &assignment,
                    const arma::mat &points);
};

}

#endif
