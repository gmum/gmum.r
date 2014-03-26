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
  while(singleLoop() > 0);
}

int Hartigan::singleLoop() {
  int switched = 0;
  for(int i = 0; i < Cluster::numberOfPoints; i++) {
    source = fits[i];
    NumericVector point = points(i,_);
    for(int k = 0; k < clusters.size(); k++)
      if(k != source && clusters[k].size > 0) {
	Cluster &oldSource = clusters[source];
	Cluster &oldTarget = cluster[k];
	Cluster newSource = clusters[source].removePoint(point);
	Cluster newTarget = clusters[k].addPoint(point);
	float oldEntropy = oldSource.entropy()+oldTarget.entropy();
	float newEntropy = newSource.entropy()+newTarget.entropy();
	if(newEntropy < oldEntropy) {
	  cluster[source] = newSource;
	  cluster[k] = newTarget;
	  switched++;

	  if(cluster[source].size < killThreshold*Cluster::numberOfPoints) {
	    //put elemenents into other clusters
	    //delete cluster
	    //but how excactly?
	  }
	}
      }
  }
  return switched;
}

double Hartigan::energy() {

}
