#ifndef CECCONFIGURATION_HPP
#define CECCONFIGURATION_HPP

#include "params.hpp"

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

#ifdef RCPP_INTERFACE
#define GMUM_ERROR(x) Rcpp::stop(x);
#define GMUM_WARNING(x) Rcpp::Rcerr << (x);
#else
#define GMUM_ERROR(x) throw x;
#define GMUM_WARNING(x) std::cerr << x;
#endif

class CecConfiguration {
private:
	gmum::Params m_params;
	// Rcpp::Function func; 
#ifdef RCPP_INTERFACE
	void set_mix_handle_standard_cluster(Rcpp::List& list);
	void set_mix_handle_fixed_covariance_cluster(Rcpp::List &list);
	void set_mix_handle_fixed_spherical_cluster(Rcpp::List &list);
	void set_mix_handle_spherical_cluster(Rcpp::List &list);
	void set_mix_handle_diagonal_cluster(Rcpp::List& list);
	void set_mix_handle_custom_cluster(Rcpp::List& list);
#endif

public:
    CecConfiguration();

    gmum::Params& get_params();
	void set_params(gmum::Params params);

	void set_eps(const double kill_threshold);

	void set_nclusters(const unsigned int nclusters);
	void set_log_energy(bool log_energy);
	void set_log_cluster(bool log_nclusters);
	void set_nstart(const unsigned int nstart);

#ifdef RCPP_INTERFACE
	void set_mix(const Rcpp::List clusters);
	void set_centroids(const Rcpp::List centroids);
	void set_data_set(const Rcpp::NumericMatrix proxy_dataset);
	void set_cov(const Rcpp::NumericMatrix cov_mat_proxy);
	void set_function(Rcpp::Function function);
#endif

	void set_method_init(const std::string init);
	void set_method_type(const std::string type);

	void set_r(const double radius);

	void set_it_max(int it_max);
    void set_algorithm(const std::string algorithm);

};

#endif /* CECCONFIGURATION_HPP */
