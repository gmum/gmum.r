#include "CEC.hpp"

CEC::CEC(arma::mat &points, std::vector<unsigned int> &assignment,
	 float killThreshold, Algorithm *algorithm,
	 std::vector<ClusterType> &type, std::vector<float> &radius,
	 std::vector<arma::mat> &covMatrices)
  : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

  clusters.reserve(type.size());

  for(unsigned int i = 0; i < type.size(); i++) {
    Cluster cluster;
    switch(type[i]) {
    case usual:
      cluster = Cluster(i, assignment, points);
      break;
    case covMatrix:
      cluster = ClusterCovMat(covMatrices[i], i, assignment, points);
      break;
    case constRadius:
      cluster = ClusterConstRadius(radius[i], i, assignment, points);
      break;
    case spherical:
      cluster = ClusterSpherical(i, assignment, points);
      break;
    case diagonal:
      cluster = ClusterDiagonal(i, assignment, points);
      break;
    }
    clusters.push_back(cluster);
  }

  Cluster::numberOfPoints = points.n_rows;
}

CEC::CEC(arma::mat &points, std::vector<unsigned int> &assignment,
	 float killThreshold, Algorithm *algorithm, int numberOfClusters)
  : assignment(assignment), points(points), algorithm(algorithm), killThreshold(killThreshold) {

  clusters.reserve(numberOfClusters);
  for(int i = 0; i < numberOfClusters; i++)
    clusters.push_back(Cluster(i, assignment, points));

  Cluster::numberOfPoints = points.n_rows;
}

void CEC::loop() {
  algorithm->loop(points, assignment, killThreshold, clusters);
}

int CEC::singleLoop() {
  return algorithm->singleLoop(points, assignment, killThreshold, clusters);
}

float CEC::entropy() {
  float s= 0.0;
  for (std::vector<Cluster>::iterator it = clusters.begin() ; it!= clusters.end();++it )
    s+= it->entropy();
  return s;
}
