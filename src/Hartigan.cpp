#include "Hartigan.hpp"

Hartigan::Hartigan(int numberOfClusters, float killThreshold,
	 std::vector<unsigned int> &fits, arma::mat &points)
  : fits(fits), points(points), killThreshold(killThreshold) {

  clusters.reserve(numberOfClusters);
  for(int i = 0; i < numberOfClusters; i++)
    clusters.push_back(Cluster(i, fits, points));

  Cluster::numberOfPoints = points.n_rows;
}

void Hartigan::loop() {
  while(singleLoop() > 0);
}

int Hartigan::singleLoop() {
  int switched = 0;
  for(int i = 0; i < Cluster::numberOfPoints; i++) {
    unsigned int source = fits[i];
    arma::rowvec point = points.row(i);
    for(unsigned int k = 0; k < clusters.size(); k++)
      if(k != source && clusters[k].size() > 0) {
	Cluster &oldSource = clusters[source];
	Cluster &oldTarget = clusters[k];
	Cluster newSource = clusters[source].removePoint(point);
	Cluster newTarget = clusters[k].addPoint(point);
	float oldEntropy = oldSource.entropy()+oldTarget.entropy();
	float newEntropy = newSource.entropy()+newTarget.entropy();
	if(newEntropy < oldEntropy) {
	  clusters[source] = newSource;
	  clusters[k] = newTarget;
	  switched++;

	  if(clusters[source].size() < killThreshold*Cluster::numberOfPoints) {
	    //put elemenents into other clusters
	    //delete cluster
	    //but how excactly?
	  }
	}
      }
  }
  return switched;
}

double Hartigan::entropy() {
  return 0;
}
