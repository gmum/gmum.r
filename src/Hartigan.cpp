#include "Hartigan.hpp"

Hartigan(int numberOfClusters, float killThreshold,
	 std::vector<int> &fits, Rcpp::NumericMatrix &points)
  : fits(fits), points(points), killThreshold(killThreshold) {

  clusters.reserve(numberOfClusters);
  for(int i = 0; i < numberOfClusters; i++)
    clusters.push_back(Cluster(i, fits, points));

  Cluster::numberOfPoints = points.nrow();
}

void Hartigan::loop() {

}

void Hartigan::singleLoop() {

}

double Hartigan::energy() {

}
