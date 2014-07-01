#ifndef PARAMS_HPP
#define PARAMS_HPP

namespace gmum {

  enum AssignmentType {
    kkmeanspp, krandom, kcentroids, knoAssignment,
  };

  struct Params {
    bool datasetSet;
    boost::shared_ptr<const arma::mat> dataset;
    double killThreshold;
    int nrOfClusters;
    bool logNrOfClusters, logEnergy;
    int nstart;
    AssignmentType assignmentType;
    bool centroidsSet;
    std::list<std::vector<double> > centroids;
    ClusterType clusterType;
    std::list<boost::shared_ptr<ClusterParams> > clusters;
    bool covMatSet;
    arma::mat covMat;
    bool radiusSet;
    double radius;
  };

}

#endif
