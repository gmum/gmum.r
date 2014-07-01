#include "cec.hpp"

namespace gmum {

  boost::shared_ptr<Cluster> CEC::createCluster(const ClusterParams &params, int i) {

    boost::shared_ptr<Cluster> cluster;
    switch(params.type) {
    case kstandard:
      cluster = boost::shared_ptr<Cluster>(new Cluster(i, *assignment, *points));
      break;
    case kfull: {
      const ClusterFullParams &ptr = static_cast<const ClusterFullParams&>(params);
      cluster = boost::shared_ptr<Cluster>(new ClusterCovMat(ptr.covMat, i, *assignment, *points));
      break;
    }
    case kdiagonal:
      cluster = boost::shared_ptr<Cluster>(new ClusterDiagonal(i, *assignment, *points));
      break;
    case ksphere:
      cluster = boost::shared_ptr<Cluster>(new ClusterSpherical(i, *assignment, *points));
      break;
    case kfsphere:
      const ClusterFsphereParams &ptr = static_cast<const ClusterFsphereParams&>(params);
      cluster = boost::shared_ptr<Cluster>(new ClusterConstRadius(ptr.radius, i, *assignment, *points));
      break;
    }

    return cluster;
  }

  CEC::CEC(boost::shared_ptr<Algorithm> algorithm,
	   boost::shared_ptr<std::vector<unsigned int> > assignment,
	   const Params &params)
    : algorithm(algorithm), assignment(assignment), 
      points(params.dataset), killThreshold(params.killThreshold) {

    Cluster::numberOfPoints = points->n_rows;
    clusters.reserve(params.nrOfClusters);

    int i=0;
    if(params.clusterType == kmix)
      BOOST_FOREACH(boost::shared_ptr<ClusterParams> cluster, params.clusters) {
	clusters.push_back(createCluster(*cluster, i));
      }
    else {
      ClusterParams *cluster;
      switch(params.clusterType) {
      case kfsphere: {
	ClusterFsphereParams *proxy = new ClusterFsphereParams();
	proxy->radius = params.radius;
	cluster = proxy;
	break;
      }
      case kfull: {
	ClusterFullParams *proxy = new ClusterFullParams();
	proxy->covMat = params.covMat;
	cluster = proxy;
	break;
      }
      default:
	/*case standard:
	  case diagonal:
	  case sphere:*/
	cluster = new ClusterParams();
	cluster->type = params.clusterType;
	  break;
      }
      for(int i=0; i<params.nrOfClusters; ++i)
	clusters.push_back(createCluster(*cluster, i));
      delete cluster;
    }

  }

  void CEC::loop() {
    result = algorithm->loop(*points, *assignment, killThreshold, clusters);
    for(int i=0; i<clusters.size(); ++i) {
      ClusterOnlyTrace *ptr = dynamic_cast<ClusterOnlyTrace*>(&(*clusters[i]));
      if(ptr != 0) ptr->computeCovarianceMatrix(i, *assignment, *points);
    }
  }

  void CEC::singleLoop() {
    algorithm->singleLoop(*points, *assignment, killThreshold, clusters);
  }

  double CEC::entropy() {
    double s= 0.0;
    BOOST_FOREACH(boost::shared_ptr<Cluster> cluster, clusters) {
      s+= cluster->entropy();
    }
    return s;
  }

  std::vector<unsigned int> CEC::getAssignment() const {
    return *assignment;
  }

  std::vector<arma::rowvec> CEC::centers() const {
    std::vector<arma::rowvec> array;
    array.reserve(clusters.size());
    for(int i=0; i<clusters.size(); ++i) array.push_back(clusters[i]->getMean());
    return array;
  }

  std::vector<arma::mat> CEC::cov() const {
    std::vector<arma::mat> array;
    array.reserve(clusters.size());

    for(int i=0; i<clusters.size(); ++i) array.push_back(clusters[i]->getCovMat());

    return array;
  }

  unsigned int CEC::iters() const {
    return result.iterations;
  }

  std::list<unsigned int> CEC::getNrOfClusters() const {
    return result.nrOfClusters;
  }

  std::list<double> CEC::getEnergy() const {
    return result.energy;
  }

  boost::shared_ptr<const arma::mat> CEC::getPtrToPoints() const {
    return this->points;
  }

  boost::shared_ptr<std::vector<unsigned int> > CEC::getPtrToAssignement() const {
    return this->assignment;
  }
  
}
