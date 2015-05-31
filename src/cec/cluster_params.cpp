#include "params.hpp"

using namespace gmum;

ClusterParams::ClusterParams(ClusterType _type) :
    type(_type) {}
ClusterParams::~ClusterParams() {}


ClusterFullParams::ClusterFullParams(arma::mat _cov_mat) :
    ClusterParams(kfull), cov_mat_set(true), cov_mat(_cov_mat) {}

ClusterFullParams::ClusterFullParams() :
    ClusterParams(kfull), cov_mat_set(false) {}

ClusterFullParams::~ClusterFullParams() {}


ClusterFsphereParams::ClusterFsphereParams(double _radius) :
    ClusterParams(kfsphere), radius_set(true), radius(_radius) {}

ClusterFsphereParams::ClusterFsphereParams() :
    ClusterParams(kfsphere), radius_set(false) {}

ClusterFsphereParams::~ClusterFsphereParams() {}


ClusterCustomParams::ClusterCustomParams(
    boost::shared_ptr<Rcpp::Function> _function) :
    ClusterParams(kcustom), function(_function) {}

ClusterCustomParams::ClusterCustomParams() : ClusterParams(kcustom) {}

ClusterCustomParams::~ClusterCustomParams() {}

