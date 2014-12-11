#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

#include <RcppArmadillo.h>
#include <string>
namespace gmum {

enum ClusterType {
	kstandard, kfull, kdiagonal, ksphere, kfsphere, knoType, kmix, kcustom
};

/**
 * You need to store somewhere data about cluster. Each type can have its own.
 */
struct ClusterParams {
	ClusterType type;
	//virtual ~ClusterParams(){};
};

struct ClusterFullParams: public ClusterParams {
	bool covMatSet;
	arma::mat covMat;
};

struct ClusterFsphereParams: public ClusterParams {
	bool radiusSet;
	double radius;
};

struct ClusterCustomParams: public ClusterParams {
	bool functionNameSet;
	std::string functionName;
};

}

#endif
