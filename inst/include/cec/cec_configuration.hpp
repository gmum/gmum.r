#ifndef CECCONFIGURATION_HPP
#define CECCONFIGURATION_HPP

#include "params.hpp"

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

#ifdef RCPP_INTERFACE
#define GMUM_ERROR(x) Rcpp::stop(x);
#else
#define GMUM_ERROR(x) throw x;
#endif

class CecConfiguration {
private:
    gmum::Params m_params;
public:
    CecConfiguration();
    gmum::Params get_params();
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
#endif

    void set_method_init(const std::string init);
    void set_method_type(const std::string type);

    void set_r(const double radius);
    void set_function(const std::string function_name);
    void set_it_max(const unsigned int it_max);
    void set_iters(bool iters);
};

#endif /* CECCONFIGURATION_HPP */