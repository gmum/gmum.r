#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <RcppArmadillo.h>
#include <string>
namespace gmum {

enum ClusterType {
    kstandard, kfull, kdiagonal, ksphere, kfsphere, kno_type, kmix, kcustom
};

/**
 * You need to store somewhere data about cluster. Each type can have its own.
 */
struct ClusterParams {
	ClusterType type;
};

struct ClusterFullParams: public ClusterParams {
    bool cov_mat_set;
    arma::mat cov_mat;
};

struct ClusterFsphereParams: public ClusterParams {
    bool radius_set;
	double radius;
};

struct ClusterCustomParams: public ClusterParams {
    bool function_name_set;
    std::string function_name;
};

}

#endif
