#include "CEC.hpp"

namespace gmum {

  boost::shared_ptr<Cluster> CEC::createCluster(ClusterParams &params, int i) {

    boost::shared_ptr<Cluster> cluster;
    switch(params.type) {
    case standard:
      cluster = boost::shared_ptr<Cluster>(new Cluster(i, *assignment, *points));
      break;
    case full: {
      ClusterFullParams &ptr = static_cast<ClusterFullParams&>(params);
      cluster = boost::shared_ptr<Cluster>(new ClusterCovMat(ptr.covMat, i, *assignment, *points));
      break;
    }
    case diagonal:
      cluster = boost::shared_ptr<Cluster>(new ClusterDiagonal(i, *assignment, *points));
      break;
    case sphere:
      cluster = boost::shared_ptr<Cluster>(new ClusterSpherical(i, *assignment, *points));
      break;
    case fsphere:
      ClusterFsphereParams &ptr = static_cast<ClusterFsphereParams&>(params);
      cluster = boost::shared_ptr<Cluster>(new ClusterConstRadius(ptr.radius, i, *assignment, *points));
      break;
    }

    return cluster;
  }

  CEC::CEC(boost::shared_ptr<Algorithm> algorithm,
	   boost::shared_ptr<std::vector<unsigned int> > assignment,
	   const Params params)
    : algorithm(algorithm), assignment(assignment), 
      points(params.dataset), killThreshold(params.killThreshold) {

    Cluster::numberOfPoints = points->n_rows;
    clusters.reserve(params.nrOfClusters);

    int i=0;
    if(params.clusterType == mix)
      for(std::list<boost::shared_ptr<ClusterParams> >::const_iterator it = params.clusters.begin();
	  it != params.clusters.end(); ++it, ++i)
	clusters.push_back(createCluster(*(*it), i));
    else {
      ClusterParams *cluster;
      switch(params.clusterType) {
      case fsphere: {
	ClusterFsphereParams *proxy = new ClusterFsphereParams();
	proxy->radius = params.radius;
	cluster = proxy;
	break;
      }
      case full: {
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

  CEC::CEC(boost::shared_ptr<const arma::mat> points, 
	   boost::shared_ptr<std::vector<unsigned int> > assignment, 
	   boost::shared_ptr<Algorithm> algorithm,
	   double killThreshold, int numberOfClusters)
    : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

    Cluster::numberOfPoints = points->n_rows;
    clusters.reserve(numberOfClusters);
    for(int i = 0; i < numberOfClusters; i++)
      clusters.push_back(boost::shared_ptr<Cluster>(new Cluster(i, *assignment, *points)));
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
    for (std::vector<boost::shared_ptr<Cluster> >::iterator it = clusters.begin() ; it!= clusters.end();++it ){
      s+= (*it)->entropy();
    }
    return s;
  }

  std::vector<unsigned int> CEC::getAssignment() {
    return *assignment;
  }

  std::vector<arma::rowvec> CEC::centers() {
    std::vector<arma::rowvec> array;
    array.reserve(clusters.size());
    for(int i=0; i<clusters.size(); ++i) array.push_back(clusters[i]->getMean());
    return array;
  }

  std::vector<arma::mat> CEC::cov() {
    std::vector<arma::mat> array;
    array.reserve(clusters.size());

    for(int i=0; i<clusters.size(); ++i) array.push_back(clusters[i]->getCovMat());

    return array;
  }

  unsigned int CEC::iters() {
    return result.iterations;
  }

  std::list<unsigned int> CEC::getNrOfClusters() {
    return result.nrOfClusters;
  }

  std::list<double> CEC::getEnergy() {
    return result.energy;
  }

  boost::shared_ptr<const arma::mat> CEC::getPtrToPoints() const {
    return this->points;
  }

  boost::shared_ptr<std::vector<unsigned int> > CEC::getPtrToAssignement() const {
    return this->assignment;
  }
  
}
