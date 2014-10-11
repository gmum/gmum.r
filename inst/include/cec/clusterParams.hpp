#include <armadillo>
#include <string>
#ifndef CLUSTERPARAMS_HPP
#define CLUSTERPARAMS_HPP

namespace gmum {

  enum ClusterType {
    kstandard, kfull, kdiagonal, ksphere, kfsphere, knoType, kmix, kcustom
  };

  /**
   * you need to store somewhere data about cluster. Each type can has its own.
   */
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

  struct ClusterCustomParams : public ClusterParams {
    bool functionNameSet;
    std::string functionName;
  };

}

#endif
