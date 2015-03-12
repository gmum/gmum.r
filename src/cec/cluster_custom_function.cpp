#include "cluster_custom_function.hpp"

namespace gmum {

#ifdef RCPP_INTERFACE

ClusterCustomFunction::ClusterCustomFunction(int count,
		const arma::rowvec& mean, const arma::mat& cov_mat,
		boost::shared_ptr<Rcpp::Function> function) :
ClusterUseCovMat(count, mean, cov_mat), m_function(function) {
	m_entropy = calculate_entropy(m_n, cov_mat);
}

ClusterCustomFunction::ClusterCustomFunction(unsigned int id,
		const std::vector<unsigned int> &assignment, const arma::mat &points,
		boost::shared_ptr<Rcpp::Function> function) :
ClusterUseCovMat(id, assignment, points), m_function(function) {
	m_entropy = calculate_entropy(m_n, m_cov_mat);
}

double ClusterCustomFunction::calculate_entropy(int n,
		const arma::mat &cov_mat) {

	return Rcpp::as<double>(
			(*m_function)(Rcpp::Named("m", Rcpp::wrap(n)),
					Rcpp::Named("sigma", Rcpp::wrap(cov_mat))));
}

ClusterCustomFunction* ClusterCustomFunction::clone()
{
	return new ClusterCustomFunction(m_count, m_mean, m_cov_mat, m_function);
}

#endif

}
