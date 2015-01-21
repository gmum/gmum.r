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

double ClusterUseCovMat::entropy_after_add_point(
        const arma::rowvec &point) {
    arma::rowvec r = m_mean - point;
    arma::rowvec nmean = ((m_count) * m_mean + point) / (m_count + 1);
    auto w = ((1.0 * (m_count) / (1+m_count))
            * (m_cov_mat + (r.t() * r) / (m_count+1)));
    return calculate_entropy(arma::Mat<double>((1.0 * (m_count) / (1+m_count))
            * (m_cov_mat + (r.t() * r) / (m_count+1))), m_n);
}

double ClusterUseCovMat::entropy_after_remove_point(
        const arma::rowvec &point) {
    arma::rowvec r = m_mean - point;
    arma::rowvec nmean = ((m_count) * m_mean - point) / (m_count - 1);
    return calculate_entropy(arma::Mat<double>((1.0 * (m_count) / (m_count-1))
            * (m_cov_mat - (r.t() * r) / (m_count-1))), m_n);
}


void ClusterUseCovMat::add_point(
        const arma::rowvec &point) {
    m_count = m_count + 1;
    arma::rowvec r = m_mean - point;
    m_mean = ((m_count - 1) * m_mean + point) / (m_count);
    m_cov_mat = (1.0 * (m_count - 1) / m_count)
            * (m_cov_mat + (r.t() * r) / m_count);

    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

void ClusterUseCovMat::remove_point(
        const arma::rowvec &point) {
	m_count = m_count - 1;

    arma::rowvec r = m_mean - point;
    m_mean = ((m_count + 1)* m_mean - point) / m_count;
    m_cov_mat = (1.0 * (m_count + 1) / m_count)
            * (m_cov_mat - (r.t() * r) / m_count);
    m_entropy = calculate_entropy(m_cov_mat, m_n);
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


void ClusterOnlyTrace::add_point(
        const arma::rowvec & point) {
    int ncount = m_count + 1;
    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
    arma::rowvec r = nmean - point;
    arma::rowvec mean_diff = m_mean - nmean;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count + dot(r, r))
            / ncount;
    m_cov_mat_trace = ntrace;
    m_count = ncount;
    m_mean = nmean;
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
}

double ClusterOnlyTrace::entropy_after_remove_point(
        const arma::rowvec &point) {
    int ncount = m_count - 1;
    arma::rowvec nmean = (m_count * m_mean - point) / (ncount);
    arma::rowvec mean_diff = m_mean - nmean;
    arma::rowvec r = nmean - point;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count - dot(r, r))
            / ncount;
    return calculate_entropy(ntrace, m_n);
}

double ClusterOnlyTrace::entropy_after_add_point(
        const arma::rowvec & point) {
    int ncount = m_count + 1;
    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
    arma::rowvec r = nmean - point;
    arma::rowvec mean_diff = m_mean - nmean;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count + dot(r, r))
            / ncount;
    return calculate_entropy(ntrace, m_n);
}

void ClusterOnlyTrace::remove_point(
        const arma::rowvec &point) {
    int ncount = m_count - 1;
    arma::rowvec nmean = (m_count * m_mean - point) / (ncount);
    arma::rowvec mean_diff = m_mean - nmean;
    arma::rowvec r = nmean - point;
    double ntrace =
            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count - dot(r, r))
            / ncount;
    m_cov_mat_trace = ntrace;
    m_count = ncount;
    m_mean = nmean;
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
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

double ClusterStandard::calculate_entropy(const arma::mat & cov_mat, int n)
 {
    return n * log(2 * M_PI * 	M_E) / 2 + log(arma::det(cov_mat)) / 2;
}


ClusterStandard::ClusterStandard(int count, const arma::rowvec &_mean,
                                 const arma::mat &cov_mat) :
    ClusterUseCovMat(count, _mean, cov_mat) {
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

ClusterStandard::ClusterStandard(unsigned int id,
                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

ClusterCovMat::ClusterCovMat(const arma::mat & sigma, unsigned int id,
                             const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    m_sigma_det = arma::det(sigma);
    m_inv_sigma = arma::inv(sigma);
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

ClusterCovMat::ClusterCovMat(const arma::mat& inv_sigma, double sigma_det, int count, const arma::rowvec & mean, const arma::mat & cov_mat)
    : ClusterUseCovMat(count, mean, cov_mat), m_inv_sigma(inv_sigma), m_sigma_det(sigma_det) {
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

double ClusterCovMat::calculate_entropy(const arma::mat & cov_mat, int n){
    return n * log(2 * M_PI) / 2 + arma::trace(m_inv_sigma * cov_mat) / 2
            + n * log(m_sigma_det) / 2;
}


ClusterConstRadius::ClusterConstRadius(double r, unsigned int id,
                                       const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterOnlyTrace(id, assignment, points), m_r(r) {
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
}

ClusterConstRadius::ClusterConstRadius(double r, int count,
                                       const arma::rowvec & mean, double cov_mat_trace) :
    ClusterOnlyTrace(count, mean, cov_mat_trace), m_r(r) {
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
}

double ClusterConstRadius::calculate_entropy(double cov_mat_trace, int n) {
    return n * log(2 * M_PI) / 2 + cov_mat_trace / (2 * m_r) + n * log(m_r) / 2;
}


ClusterSpherical::ClusterSpherical(unsigned int id,
                                   const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterOnlyTrace(id, assignment, points) {
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
}

ClusterSpherical::ClusterSpherical(int count, const arma::rowvec &mean,
                                   double cov_mat_trace) :
    ClusterOnlyTrace(count, mean, cov_mat_trace) {
    m_entropy = calculate_entropy(m_cov_mat_trace, m_n);
}

double ClusterSpherical::calculate_entropy(double cov_mat_trace, int n) {
    return n * log(2 * M_PI * M_E / n) / 2 + n * log(cov_mat_trace) / 2;
}


ClusterDiagonal::ClusterDiagonal(unsigned int id,
                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
    ClusterUseCovMat(id, assignment, points) {
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

ClusterDiagonal::ClusterDiagonal(int count, const arma::rowvec & mean,
                                 const arma::mat & cov_mat) :
    ClusterUseCovMat(count, mean, cov_mat) {
    m_entropy = calculate_entropy(m_cov_mat, m_n);
}

double ClusterDiagonal::calculate_entropy(const arma::mat & cov_mat, int n) {
    return  n * log(2 * M_PI * M_E) / 2
            + log(arma::det(arma::diagmat(cov_mat))) / 2;
}



}













//#include "cluster.hpp"
//
//namespace gmum {
//
//Cluster::Cluster(int count, const arma::rowvec &mean) :
//    m_count(count), m_mean(mean) {
//    m_n = m_mean.n_elem;
//}
//
//Cluster::Cluster(unsigned int id, const std::vector<unsigned int> &assignment,
//                 const arma::mat &points) {
//    initialize_mean(id, assignment, points);
//    if (m_count == 0)
//        throw(NoPointsInCluster());
//
//    m_n = points.n_cols;
//}
//
//arma::rowvec Cluster::initialize_mean(unsigned int id,
//                                      const std::vector<unsigned int> &assignment, const arma::mat &points) {
//    int dimention = points.n_cols;
//    m_count = 0;
//    m_mean = arma::rowvec(dimention, arma::fill::zeros);
//    for (unsigned int i = 0; i < points.n_rows; i++) {
//        if (assignment[i] == id) {
//            m_mean += points.row(i);
//            m_count += 1;
//        }
//    }
//    m_mean = m_mean / m_count;
//
//    return m_mean;
//}
//
//int Cluster::size() const {
//    return m_count;
//}
//
//arma::rowvec Cluster::get_mean() {
//    return m_mean;
//}
//
//double Cluster::entropy() const {
//    return m_entropy;
//}
//
//void ClusterUseCovMat::initialize_cov_mat(unsigned int id,
//                                          const std::vector<unsigned int> &assignment, const arma::mat &points) {
//    int dimension = points.n_cols;
//    arma::rowvec m = m_mean;
//    arma::mat out(dimension, dimension, arma::fill::zeros);
//    for (unsigned int i = 0; i < points.n_rows; i++)
//        if (assignment[i] == id) {
//            arma::rowvec point = points.row(i);
//            arma::rowvec tmp = point - m;
//            out += (tmp.t() * tmp) / (m_count);
//        }
//
//    m_cov_mat = out;
//}
//
//ClusterUseCovMat::ClusterUseCovMat(int count, const arma::rowvec &mean,
//                                   const arma::mat &cov_mat) :
//    Cluster(count, mean), m_cov_mat(cov_mat) {
//}
//
//ClusterUseCovMat::ClusterUseCovMat(unsigned int id,
//                                   const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    Cluster(id, assignment, points) {
//    initialize_cov_mat(id, assignment, points);
//}
//
//Cluster *ClusterUseCovMat::add_point(
//        const arma::rowvec &point) {
//    int ncount = m_count + 1;
//    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
//    arma::rowvec r = nmean - point;
//    arma::mat ncov_mat = (1.0 * m_count / ncount)
//            * (m_cov_mat + (r.t() * r) / ncount);
//    return create_instance(ncount, nmean, ncov_mat);
//}
//
//Cluster *ClusterUseCovMat::remove_point(
//        const arma::rowvec &point) {
//    int ncount = m_count - 1;
//    arma::rowvec nmean = (m_count * m_mean - point) / ncount;
//    arma::rowvec r = m_mean - point;
//    arma::mat ncov_mat = (1.0 * m_count / ncount)
//            * (m_cov_mat - (r.t() * r) / ncount);
//    return create_instance(ncount, nmean, ncov_mat);
//}
//
//arma::mat ClusterUseCovMat::get_cov_mat(unsigned int id,
//                                        const std::vector<unsigned int> &assignment, const arma::mat &points) {
//    return m_cov_mat;
//}
//
//ClusterOnlyTrace::ClusterOnlyTrace(int count, const arma::rowvec & mean,
//                                   double cov_mat_trace) :
//    Cluster(count, mean), m_cov_mat_trace(cov_mat_trace) {
//}
//
//ClusterOnlyTrace::ClusterOnlyTrace(unsigned int id,
//                                   const std::vector<unsigned int> & assignment, const arma::mat & points) :
//    Cluster(id, assignment, points) {
//    initialize_mean(id, assignment, points);
//    compute_cov_mat_trace(id, assignment, points);
//}
//
//void ClusterOnlyTrace::compute_cov_mat_trace(unsigned int id,
//                                             const std::vector<unsigned int> &assignment, const arma::mat &points) {
//    m_cov_mat_trace = 0.0;
//    for (unsigned int i = 0; i < points.n_rows; i++)
//        if (assignment[i] == id) {
//            arma::rowvec point = points.row(i);
//            arma::rowvec tmp = point - m_mean;
//            m_cov_mat_trace += dot(tmp, tmp);
//        }
//    m_cov_mat_trace /= m_count;
//}
//
//Cluster *ClusterOnlyTrace::add_point(
//        const arma::rowvec & point) {
//    int ncount = m_count + 1;
//    arma::rowvec nmean = (m_count * m_mean + point) / (ncount);
//    arma::rowvec r = nmean - point;
//    arma::rowvec mean_diff = nmean - nmean;
//    double ntrace =
//            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count + dot(r, r))
//            / ncount;
//    return create_instance(ncount, nmean, ntrace);
//}
//
//Cluster * ClusterOnlyTrace::remove_point(
//        const arma::rowvec &point) {
//    int ncount = m_count - 1;
//    arma::rowvec nmean = (m_count * m_mean - point) / (ncount);
//    arma::rowvec mean_diff = m_mean - nmean;
//    arma::rowvec r = nmean - point;
//    double ntrace =
//            ((m_cov_mat_trace + dot(mean_diff, mean_diff)) * m_count - dot(r, r))
//            / ncount;
//    return create_instance(ncount, nmean, ntrace);
//}
//
//double ClusterOnlyTrace::get_cov_mat_trace() {
//    return m_cov_mat_trace;
//}
//
//arma::mat ClusterOnlyTrace::get_cov_mat(unsigned int id,
//                                        const std::vector<unsigned int> &assignment, const arma::mat &points) {
//    arma::mat out(m_n, m_n, arma::fill::zeros);
//    for (unsigned int i = 0; i < points.n_rows; i++) {
//        if (assignment[i] == id) {
//            arma::rowvec point = points.row(i);
//            arma::rowvec tmp = point - m_mean;
//            out += (tmp.t() * tmp) / (m_count);
//        }
//    }
//    return out;
//}
//
//void ClusterStandard::calculate_entropy() {
//    m_entropy = m_n * log(2 * M_PI * M_E) / 2 + log(arma::det(m_cov_mat)) / 2;
//}
//
//double ClusterStandard::entropy_after_point_add(const arma::rowvec &point) const{
//    int ncount = m_count + 1;
//    arma::rowvec r = (m_count * m_mean + point) / (ncount) - point;
//    arma::mat ncov_mat = (1.0 * m_count / ncount)
//            * (m_cov_mat + (r.t() * r) / ncount);
//
//    return m_n * log(2 * M_PI * M_E) / 2 + log(arma::det(ncov_mat)) / 2;
//}
//
//double ClusterStandard::entropy_after_point_remove(const arma::rowvec &point) const{
//	int ncount = m_count - 1;
//	arma::rowvec r = (m_count * m_mean - point) / ncount - point;
//	arma::mat ncov_mat = (1.0 * m_count / ncount)
//	        * (m_cov_mat - (r.t() * r) / ncount);
//    return m_n * log(2 * M_PI * M_E) / 2 + log(arma::det(ncov_mat)) / 2;
//}
//
//
//
//ClusterStandard * ClusterStandard::create_instance(int count,
//                                                                     const arma::rowvec &mean, const arma::mat &cov_mat) {
//    return new ClusterStandard(count, mean, cov_mat);
//}
//
//ClusterStandard::ClusterStandard(int count, const arma::rowvec &_mean,
//                                 const arma::mat &cov_mat) :
//    ClusterUseCovMat(count, _mean, cov_mat) {
//    calculate_entropy();
//}
//
//ClusterStandard::ClusterStandard(unsigned int id,
//                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    ClusterUseCovMat(id, assignment, points) {
//    calculate_entropy();
//}
//
//ClusterCovMat::ClusterCovMat(const arma::mat & sigma, unsigned int id,
//                             const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    ClusterUseCovMat(id, assignment, points) {
//    m_sigma_det = arma::det(sigma);
//    m_inv_sigma = arma::inv(sigma);
//    calculate_entropy();
//}
//
//ClusterCovMat::ClusterCovMat(const arma::mat& inv_sigma, double sigma_det, int count, const arma::rowvec & mean, const arma::mat & cov_mat)
//    : ClusterUseCovMat(count, mean, cov_mat), m_inv_sigma(inv_sigma), m_sigma_det(sigma_det) {
//    calculate_entropy();
//}
//
//void ClusterCovMat::calculate_entropy() {
//    m_entropy = m_n * log(2 * M_PI) / 2 + arma::trace(m_inv_sigma * m_cov_mat) / 2
//            + m_n * log(m_sigma_det) / 2;
//}
//
//ClusterUseCovMat * ClusterCovMat::create_instance(int count, const arma::rowvec & mean, const arma::mat & cov_mat) {
//    return (ClusterUseCovMat*)(new ClusterCovMat(m_inv_sigma, m_sigma_det, count, mean, cov_mat));
//}
//
//ClusterConstRadius::ClusterConstRadius(double r, unsigned int id,
//                                       const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    ClusterOnlyTrace(id, assignment, points), m_r(r) {
//    calculate_entropy();
//}
//
//ClusterConstRadius::ClusterConstRadius(double r, int count,
//                                       const arma::rowvec & mean, double cov_mat_trace) :
//    ClusterOnlyTrace(count, mean, cov_mat_trace), m_r(r) {
//    calculate_entropy();
//}
//
//void ClusterConstRadius::calculate_entropy() {
//    m_entropy = m_n * log(2 * M_PI) / 2 + m_cov_mat_trace / (2 * m_r) + m_n * log(m_r) / 2;
//}
//
//ClusterConstRadius * ClusterConstRadius::create_instance(int count, const arma::rowvec & mean, double cov_mat_trace) {
//    return new ClusterConstRadius(m_r, count, mean, cov_mat_trace);
//}
//
//ClusterSpherical::ClusterSpherical(unsigned int id,
//                                   const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    ClusterOnlyTrace(id, assignment, points) {
//    calculate_entropy();
//}
//
//ClusterSpherical::ClusterSpherical(int count, const arma::rowvec &mean,
//                                   double cov_mat_trace) :
//    ClusterOnlyTrace(count, mean, cov_mat_trace) {
//    calculate_entropy();
//}
//
//void ClusterSpherical::calculate_entropy() {
//    m_entropy = m_n * log(2 * M_PI * M_E / m_n) / 2 + m_n * log(m_cov_mat_trace) / 2;
//}
//
//ClusterSpherical* ClusterSpherical::create_instance(int count,
//                                                                      const arma::rowvec & mean, double cov_mat_trace) {
//    return new ClusterSpherical(count, mean, cov_mat_trace);
//}
//
//ClusterDiagonal::ClusterDiagonal(unsigned int id,
//                                 const std::vector<unsigned int> &assignment, const arma::mat &points) :
//    ClusterUseCovMat(id, assignment, points) {
//    calculate_entropy();
//}
//
//ClusterDiagonal::ClusterDiagonal(int count, const arma::rowvec & mean,
//                                 const arma::mat & cov_mat) :
//    ClusterUseCovMat(count, mean, cov_mat) {
//    calculate_entropy();
//}
//
//void ClusterDiagonal::calculate_entropy() {
//    m_entropy = m_n * log(2 * M_PI * M_E) / 2
//            + log(arma::det(arma::diagmat(m_cov_mat))) / 2;
//}
//
//ClusterUseCovMat * ClusterDiagonal::create_instance(int count, const arma::rowvec & mean, const arma::mat & cov_mat) {
//    return (ClusterUseCovMat*)(new ClusterDiagonal(count, mean, cov_mat));
//}
//
//}
