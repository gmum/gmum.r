#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <string>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

namespace gmum {

enum ClusterType {
	kstandard, kcovariance, kdiagonal, kspherical, kspherical_fixed_r, kno_type, kmix, kcustom
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

struct ClusterCovarianceParams : public ClusterParams {
	bool cov_mat_set;
	arma::mat cov_mat;
	ClusterCovarianceParams(arma::mat _cov_mat) :
			ClusterParams(kcovariance), cov_mat_set(true), cov_mat(_cov_mat) {
	}
	ClusterCovarianceParams() :
			ClusterParams(kcovariance), cov_mat_set(false) {
	}
    virtual ~ClusterCovarianceParams() { }
};

struct ClusterSphericalFixedRParams : public ClusterParams {
	bool radius_set;
	double radius;
	ClusterSphericalFixedRParams(double _radius) :
			ClusterParams(kspherical_fixed_r), radius_set(true), radius(_radius) {
	}
	ClusterSphericalFixedRParams() :
			ClusterParams(kspherical_fixed_r), radius_set(false) {
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
