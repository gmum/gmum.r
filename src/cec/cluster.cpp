#include "cluster.hpp"

namespace gmum {

Cluster::Cluster(int count, const arma::rowvec &mean) :
    m_count(count), m_mean(mean) {
    m_n = m_mean.n_elem;
}

Cluster::Cluster(unsigned int id, const std::vector<unsigned int> &assignment,
                 const arma::mat &points) {
    initialize_mean(id, assignment, points);
    if (m_count == 0)
        throw(NoPointsInCluster());

    m_n = points.n_cols;
}

arma::rowvec Cluster::initialize_mean(unsigned int id,
                                      const std::vector<unsigned int> &assignment, const arma::mat &points) {
    int dimention = points.n_cols;
    m_count = 0;
    m_mean = arma::rowvec(dimention, arma::fill::zeros);
    for (unsigned int i = 0; i < points.n_rows; i++) {
        if (assignment[i] == id) {
            m_mean += points.row(i);
            m_count += 1;
        }
    }
    m_mean = m_mean / m_count;

    return m_mean;
}

int Cluster::size() const {
    return m_count;
}

arma::rowvec Cluster::get_mean() {
    return m_mean;
}

double Cluster::entropy() const {
    return m_entropy;
}

void ClusterUseCovMat::initialize_cov_mat(unsigned int id,
                                          const std::vector<unsigned int> &assignment, const arma::mat &points) {
    int dimension = points.n_cols;
    arma::rowvec m = m_mean;
    arma::mat out(dimension, dimension, arma::fill::zeros);
    for (unsigned int i = 0; i < points.n_rows; i++)
        if (assignment[i] == id) {
            arma::rowvec point = points.row(i);
            arma::rowvec tmp = point - m;
            out += (tmp.t() * tmp) / (m_count);
        }

    m_cov_mat = out;
}

ClusterUseCovMat::ClusterUseCovMat(int count, const arma::rowvec &mean,
                                   const arma::mat &cov_mat) :
    Cluster(count, mean), m_cov_mat(cov_mat) {
}

ClusterUseCovMat::ClusterUseCovMat(unsigned int id,
                                   const std::vector<unsigned int> &assignment, const arma::mat &points) :
    Cluster(id, assignment, points) {
    initialize_cov_mat(id, assignment, points);
}

boost::shared_ptr<Cluster> ClusterUseCovMat::add_point(
        const arma::rowvec &point) {
    int ncount = m_count + 1;
    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
    arma::rowvec r = m_mean - point;
    arma::mat ncov_mat = (1.0 * m_count / ncount)
            * (m_cov_mat + (r.t() * r) / ncount);
    return create_instance(ncount, nmean, ncov_mat);
}

boost::shared_ptr<Cluster> ClusterUseCovMat::remove_point(
        const arma::rowvec &point) {
    int ncount = m_count - 1;
    arma::rowvec nmean = (m_count * m_mean - point) / ncount;
    arma::rowvec r = m_mean - point;
    arma::mat ncov_mat = (1.0 * m_count / ncount)
            * (m_cov_mat - (r.t() * r) / ncount);
    return create_instance(ncount, nmean, ncov_mat);
}

arma::mat ClusterUseCovMat::get_cov_mat(unsigned int id,
                                        const std::vector<unsigned int> &assignment, const arma::mat &points) {
    return m_cov_mat;
}

ClusterOnlyTrace::ClusterOnlyTrace(int count, const arma::rowvec & mean,
                                   double cov_mat_trace) :
    Cluster(count, mean), m_cov_mat_trace(cov_mat_trace) {
}

ClusterOnlyTrace::ClusterOnlyTrace(unsigned int id,
                                   const std::vector<unsigned int> & assignment, const arma::mat & points) :
    Cluster(id, assignment, points) {
    initialize_mean(id, assignment, points);
    compute_cov_mat_trace(id, assignment, points);
}

void ClusterOnlyTrace::compute_cov_mat_trace(unsigned int id,
                                             const std::vector<unsigned int> &assignment, const arma::mat &points) {
    m_cov_mat_trace = 0.0;
    for (unsigned int i = 0; i < points.n_rows; i++)
        if (assignment[i] == id) {
            arma::rowvec point = points.row(i);
            arma::rowvec tmp = point - m_mean;
            m_cov_mat_trace += dot(tmp, tmp);
        }
    m_cov_mat_trace /= m_count;
}

boost::shared_ptr<Cluster> ClusterOnlyTrace::add_point(
        const arma::rowvec & point) {
    int ncount = m_count + 1;
    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
    arma::rowvec r = nmean - point;
    arma::rowvec mean_diff = m_mean - nmean;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count + dot(r, r))
            / ncount;
    return create_instance(ncount, nmean, ntrace);
}

boost::shared_ptr<Cluster> ClusterOnlyTrace::remove_point(
        const arma::rowvec &point) {
    int ncount = m_count - 1;
    arma::rowvec nmean = (m_count * m_mean - point) / (ncount);
    arma::rowvec mean_diff = m_mean - nmean;
    arma::rowvec r = nmean - point;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count - dot(r, r))
            / ncount;
    return create_instance(ncount, nmean, ntrace);
}

double ClusterOnlyTrace::get_cov_mat_trace() {
    return m_cov_mat_trace;
}

arma::mat ClusterOnlyTrace::get_cov_mat(unsigned int id,
                                        const std::vector<unsigned int> &assignment, const arma::mat &points) {
    arma::mat out(m_n, m_n, arma::fill::zeros);
    for (unsigned int i = 0; i < points.n_rows; i++) {
        if (assignment[i] == id) {
            arma::rowvec point = points.row(i);
            arma::rowvec tmp = point - m_mean;
            out += (tmp.t() * tmp) / (m_count);
        }
    }
    return out;
}

void ClusterStandard::calculate_entropy() {
    m_entropy = m_n * log(2 * M_PI * M_E) / 2 + log(arma::det(m_cov_mat)) / 2;
}

boost::shared_ptr<ClusterUseCovMat> ClusterStandard::create_instance(int count,
                                                                     const arma::rowvec &mean, const arma::mat &cov_mat) {
    return boost::shared_ptr < ClusterUseCovMat
            > (new ClusterStandard(count, mean, cov_mat));
}

ClusterStandard::ClusterStandard(int count, const arma::rowvec &_mean,
                                 const arma::mat &cov_mat) :
    ClusterUseCovMat(count, _mean, cov_mat) {
    calculate_entropy();
}

ClusterStandard::ClusterStandard(unsigned int id,
                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    calculate_entropy();
}

ClusterCovMat::ClusterCovMat(const arma::mat & sigma, unsigned int id,
                             const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    m_sigma_det = arma::det(sigma);
    m_inv_sigma = arma::inv(sigma);
    calculate_entropy();
}

ClusterCovMat::ClusterCovMat(const arma::mat& inv_sigma, double sigma_det, int count, const arma::rowvec & mean, const arma::mat & cov_mat)
    : ClusterUseCovMat(count, mean, cov_mat), m_inv_sigma(inv_sigma), m_sigma_det(sigma_det) {
    calculate_entropy();
}

void ClusterCovMat::calculate_entropy() {
    m_entropy = m_n * log(2 * M_PI) / 2 + arma::trace(m_inv_sigma * m_cov_mat) / 2
            + m_n * log(m_sigma_det) / 2;
}
boost::shared_ptr<ClusterUseCovMat> ClusterCovMat::create_instance(int count,
                                                                   const arma::rowvec & mean, const arma::mat & cov_mat) {
    return boost::shared_ptr < ClusterUseCovMat
            > (new ClusterCovMat(m_inv_sigma, m_sigma_det, count, mean, cov_mat));
}

ClusterConstRadius::ClusterConstRadius(double r, unsigned int id,
                                       const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterOnlyTrace(id, assignment, points), m_r(r) {
    calculate_entropy();
}

ClusterConstRadius::ClusterConstRadius(double r, int count,
                                       const arma::rowvec & mean, double cov_mat_trace) :
    ClusterOnlyTrace(count, mean, cov_mat_trace), m_r(r) {
    calculate_entropy();
}

void ClusterConstRadius::calculate_entropy() {
    m_entropy = m_n * log(2 * M_PI) / 2 + m_cov_mat_trace / (2 * m_r) + m_n * log(m_r) / 2;
}

boost::shared_ptr<ClusterOnlyTrace> ClusterConstRadius::create_instance(int count, const arma::rowvec & mean, double cov_mat_trace) {
    return boost::shared_ptr < ClusterOnlyTrace
            > (new ClusterConstRadius(m_r, count, mean, cov_mat_trace));
}

ClusterSpherical::ClusterSpherical(unsigned int id,
                                   const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterOnlyTrace(id, assignment, points) {
    calculate_entropy();
}

ClusterSpherical::ClusterSpherical(int count, const arma::rowvec &mean,
                                   double cov_mat_trace) :
    ClusterOnlyTrace(count, mean, cov_mat_trace) {
    calculate_entropy();
}

void ClusterSpherical::calculate_entropy() {
    m_entropy = m_n * log(2 * M_PI * M_E / m_n) / 2 + m_n * log(m_cov_mat_trace) / 2;
}

boost::shared_ptr<ClusterOnlyTrace> ClusterSpherical::create_instance(int count,
                                                                      const arma::rowvec & mean, double cov_mat_trace) {
    return boost::shared_ptr < ClusterOnlyTrace
            > (new ClusterSpherical(count, mean, cov_mat_trace));
}

ClusterDiagonal::ClusterDiagonal(unsigned int id,
                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    calculate_entropy();
}

ClusterDiagonal::ClusterDiagonal(int count, const arma::rowvec & mean,
                                 const arma::mat & cov_mat) :
    ClusterUseCovMat(count, mean, cov_mat) {
    calculate_entropy();
}

void ClusterDiagonal::calculate_entropy() {
    m_entropy = m_n * log(2 * M_PI * M_E) / 2
            + log(arma::det(arma::diagmat(m_cov_mat))) / 2;
}

boost::shared_ptr<ClusterUseCovMat> ClusterDiagonal::create_instance(int count,
                                                                     const arma::rowvec & mean, const arma::mat & cov_mat) {
    return boost::shared_ptr < ClusterUseCovMat
            > (new ClusterDiagonal(count, mean, cov_mat));
}
}
