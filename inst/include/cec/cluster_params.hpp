#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <string>

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#endif

#ifdef SWIG
%include <boost_shared_ptr.i>
%{
#define SWIG_FILE_WITH_INIT
#include <boost/shared_ptr.hpp>
#include "cluster_params.hpp"
using namespace gmum;
using namespace boost;
%}
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
	ClusterParams(ClusterType _type);
    virtual ~ClusterParams();
};

struct ClusterFullParams: public ClusterParams {
	bool cov_mat_set;
	arma::mat cov_mat;
	ClusterFullParams(arma::mat _cov_mat);
	ClusterFullParams();
    virtual ~ClusterFullParams();
};

struct ClusterFsphereParams: public ClusterParams {
	bool radius_set;
	double radius;
	ClusterFsphereParams(double _radius);
	ClusterFsphereParams();
    virtual ~ClusterFsphereParams();
};

#ifdef RCPP_INTERFACE
struct ClusterCustomParams: public ClusterParams {
	boost::shared_ptr<Rcpp::Function> function;
	ClusterCustomParams(boost::shared_ptr<Rcpp::Function> _function);
	ClusterCustomParams();
    virtual ~ClusterCustomParams();
};
#endif

}

#endif
