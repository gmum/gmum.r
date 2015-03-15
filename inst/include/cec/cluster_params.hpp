#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <armadillo>
#include <string>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

namespace gmum {

enum ClusterType {
	kstandard, kfull, kdiagonal, ksphere, kfsphere, kno_type, kmix, kcustom
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

struct ClusterFullParams: public ClusterParams {
	bool cov_mat_set;
	arma::mat cov_mat;
	ClusterFullParams(arma::mat _cov_mat) :
			ClusterParams(kfull), cov_mat_set(true), cov_mat(_cov_mat) {
	}
	ClusterFullParams() :
			ClusterParams(kfull), cov_mat_set(false) {
	}
    virtual ~ClusterFullParams() { }
};

struct ClusterFsphereParams: public ClusterParams {
	bool radius_set;
	double radius;
	ClusterFsphereParams(double _radius) :
			ClusterParams(kfsphere), radius_set(true), radius(_radius) {
	}
	ClusterFsphereParams() :
			ClusterParams(kfsphere), radius_set(false) {
	}
    virtual ~ClusterFsphereParams() { }
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
