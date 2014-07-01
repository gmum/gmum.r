#include <armadillo>

#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

namespace gmum {

  enum ClusterType {
    kstandard, kfull, kdiagonal, ksphere, kfsphere, knoType, kmix,
  };

  struct ClusterParams {
    ClusterType type;
    //virtual ~ClusterParams(){};
  };

  struct ClusterFullParams : public ClusterParams {
    bool covMatSet;
    arma::mat covMat;
  };

  struct ClusterFsphereParams : public ClusterParams {
    bool radiusSet;
    double radius;
  };

}

#endif
