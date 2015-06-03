#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <string>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

namespace gmum {

enum ClusterType {
	kstandard, kfixed_covariance, kdiagonal, kfixed_spherical, kspherical, kno_type, kmix, kcustom
};

/**
 * You need to store somewhere data about cluster. Each type can have its own.
 */
struct ClusterParams {
	ClusterType type;
	ClusterParams(ClusterType _type) :
			type(_type) {
	}
    virtual ~ClusterParams() { };
};

struct ClusterFixedCovarianceParams : public ClusterParams {
	bool cov_mat_set;
	arma::mat cov_mat;
	ClusterFixedCovarianceParams(arma::mat _cov_mat) :
			ClusterParams(kfixed_covariance), cov_mat_set(true), cov_mat(_cov_mat) {
	}
	ClusterFixedCovarianceParams() :
			ClusterParams(kfixed_covariance), cov_mat_set(false) {
	}
    virtual ~ClusterFixedCovarianceParams() { }
};

struct ClusterSphericalFixedRParams : public ClusterParams {
	bool radius_set;
	double radius;
	ClusterSphericalFixedRParams(double _radius) :
			ClusterParams(kfixed_spherical), radius_set(true), radius(_radius) {
	}
	ClusterSphericalFixedRParams() :
			ClusterParams(kfixed_spherical), radius_set(false) {
	}
    virtual ~ClusterSphericalFixedRParams() { }
};

#ifdef RCPP_INTERFACE
struct ClusterCustomParams: public ClusterParams {
	boost::shared_ptr<Rcpp::Function> function;
	ClusterCustomParams(boost::shared_ptr<Rcpp::Function> _function) : ClusterParams(kcustom), function(_function) {}
	ClusterCustomParams() : ClusterParams(kcustom) {}
    virtual ~ClusterCustomParams() { }
};
#endif

}

#endif
