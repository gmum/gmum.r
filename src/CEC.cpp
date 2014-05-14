#include "CEC.hpp"
CEC::CEC(boost::shared_ptr<arma::mat> points, 
	 boost::shared_ptr<std::vector<unsigned int> > assignment, 
	 boost::shared_ptr<Algorithm> algorithm,
	 float killThreshold, std::vector<ClusterType> type,
	 std::vector<float> radius, std::vector<arma::mat> covMatrices)
  : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

  Cluster::numberOfPoints = points->n_rows;
  clusters.reserve(type.size());

  for(unsigned int i = 0; i < type.size(); i++) {
    Cluster cluster;
    switch(type[i]) {
    case usual:
      cluster = Cluster(i, *assignment, *points);
      break;
    case covMatrix:
      cluster = ClusterCovMat(covMatrices[i], i, *assignment, *points);
      break;
    case constRadius:
      cluster = ClusterConstRadius(radius[i], i, *assignment, *points);
      break;
    case spherical:
      cluster = ClusterSpherical(i, *assignment, *points);
      break;
    case diagonal:
      cluster = ClusterDiagonal(i, *assignment, *points);
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
    clusters.push_back(Cluster(i, *assignment, *points));
}

void CEC::loop() {
  algorithm->loop(*points, *assignment, killThreshold, clusters);
}

int CEC::singleLoop() {
  return algorithm->singleLoop(*points, *assignment, killThreshold, clusters);
}

float CEC::entropy() {
  float s= 0.0;
  for (std::vector<Cluster>::iterator it = clusters.begin() ; it!= clusters.end();++it )
    s+= it->entropy();
  return s;
}

std::vector<unsigned int> CEC::getAssignment() {
  return *assignment;
}

std::string CEC::test() {
  return "bla bla";
}
