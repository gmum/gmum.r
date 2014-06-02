#include "CEC.hpp"

namespace gmum {

  CEC::CEC(boost::shared_ptr<arma::mat> points, 
	   boost::shared_ptr<std::vector<unsigned int> > assignment, 
	   boost::shared_ptr<Algorithm> algorithm,
	   float killThreshold, std::vector<ClusterType> type,
	   std::vector<float> radius, std::vector<arma::mat> covMatrices)
    : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

    Cluster::numberOfPoints = points->n_rows;
    clusters.reserve(type.size());

    for(unsigned int i = 0; i < type.size(); i++) {
      boost::shared_ptr<Cluster> cluster;
      switch(type[i]) {
      case standard:
	cluster = boost::shared_ptr<Cluster>(new Cluster(i, *assignment, *points));
	break;
      case full:
	cluster = boost::shared_ptr<Cluster>(new ClusterCovMat(covMatrices[i], i, *assignment, *points));
	break;
      case diagonal:
	cluster = boost::shared_ptr<Cluster>(new ClusterDiagonal(i, *assignment, *points));
	break;
      case sphere:
	cluster = boost::shared_ptr<Cluster>(new ClusterSpherical(i, *assignment, *points));
	break;
      case fsphere:
	cluster = boost::shared_ptr<Cluster>(new ClusterConstRadius(radius[i], i, *assignment, *points));
	break;
      }
      clusters.push_back(cluster);
    }

  }

  CEC::CEC(boost::shared_ptr<arma::mat> points, 
	   boost::shared_ptr<std::vector<unsigned int> > assignment, 
	   boost::shared_ptr<Algorithm> algorithm,
	   float killThreshold, int numberOfClusters)
    : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

    Cluster::numberOfPoints = points->n_rows;
    clusters.reserve(numberOfClusters);
    for(int i = 0; i < numberOfClusters; i++)
      clusters.push_back(boost::shared_ptr<Cluster>(new Cluster(i, *assignment, *points)));
  }

  void CEC::loop() {
    result = algorithm->loop(*points, *assignment, killThreshold, clusters);
  }

  void CEC::singleLoop() {
    algorithm->singleLoop(*points, *assignment, killThreshold, clusters);
  }

  float CEC::entropy() {
    float s= 0.0;
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

  std::list<float> CEC::getEnergy() {
    return result.energy;
  }

}
