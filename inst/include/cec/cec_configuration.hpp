#ifndef CECCONFIGURATION_HPP
#define CECCONFIGURATION_HPP

#include <armadillo>

#include "params.hpp"

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

#ifdef RCPP_INTERFACE
#define GMUM_ERROR(x) Rcpp::stop(x);
#define GMUM_WARNING(x) std::cout << x;
#else
#define GMUM_ERROR(x) throw x;
#define GMUM_WARNING(x) std::cerr << x;
#endif

class CecConfiguration {
public:
    CecConfiguration();
    ~CecConfiguration();

    const gmum::Params& get_params() const;
	void set_params(gmum::Params params);
    void set_dataset(arma::mat& dataset);
    void set_eps(double kill_threshold);
    void set_nclusters(unsigned int nclusters);
	void set_log_energy(bool log_energy);
	void set_log_cluster(bool log_nclusters);
    void set_nstart(unsigned int nstart);
    void set_method_init(std::string init);
    void set_method_type(std::string type);
    void set_r(double radius);
    void set_it_max(int it_max);
    void set_algorithm(std::string algorithm);

#ifdef RCPP_INTERFACE
	void set_mix(const Rcpp::List clusters);
	void set_centroids(const Rcpp::List centroids);
    void set_dataset_rcpp(const Rcpp::NumericMatrix proxy_dataset);
	void set_cov(const Rcpp::NumericMatrix cov_mat_proxy);
	void set_function(Rcpp::Function function);
#endif

private:
    gmum::Params m_params;
    // Rcpp::Function func;
#ifdef RCPP_INTERFACE
    void set_mix_handle_standard_cluster(Rcpp::List& list);
    void set_mix_handle_full_cluster(Rcpp::List& list);
    void set_mix_handle_fsphere_cluster(Rcpp::List& list);
    void set_mix_handle_sphere_cluster(Rcpp::List& list);
    void set_mix_handle_diagonal_cluster(Rcpp::List& list);
    void set_mix_handle_custom_cluster(Rcpp::List& list);
#endif
};

#endif /* CECCONFIGURATION_HPP */
