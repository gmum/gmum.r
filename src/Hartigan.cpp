#include "Hartigan.hpp"

Hartigan::Hartigan(int numberOfClusters, float killThreshold,
	 std::vector<unsigned int> &fits, arma::mat &points)
  : fits(fits), points(points), killThreshold(killThreshold) {

  clusters.reserve(numberOfClusters);
  for(int i = 0; i < numberOfClusters; i++)
    clusters.push_back(Cluster(i, fits, points));
clusters
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
    for(unsigned int k = 0; k < clusters.size(); k++){
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
			  //delete cluster
			  clusters.erase(std::begin(clusters) + source);
			  //assign points of erased cluster
			  for(int j = 0; j < Cluster::numberOfPoints; j++) {
				arma::rowvec pointToAssign = points.row(j);
				//find point of deleted cluster
				if(fits[j] == source){
					int minEntropyChangeFitIndex = -1;
				    Cluster *minEntropyChangeCluster = NULL;
					float minEntropyChange = std::numeric_limits<float>::max();
					//find the best cluster to assign the point to it 
					for(unsigned int l = 0; l < clusters.size(); l++){
						if(l != source) {
							Cluster &oldTarget = clusters[l];
							Cluster newTarget = clusters[l].addPoint(pointToAssign);
							float entropyChange = oldTarget.entropy() - newTarget.entropy();
							if(entropyChange < minEntropyChange){
								minEntropyChange = entropyChange;
								minEntropyChangeElementIndex = l;
								minEntropyChangeCluster = &newTarget;						
							}
						}
					}
					//minEntropyChangeElementIndexi != -1 && &minEntropyChangeCluster != NULL
					clusters[minEntropyChangeElementIndex] = &minEntropyChangeCluster;
				}
			  }			  
		  }
		  break; //point was switched so we'll stop the clusters loop and we'll check the next point
		}
	  }
    }
  }
  return switched;
}

double Hartigan::entropy() {
  return 0;
}
