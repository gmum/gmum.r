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

  int switched = 0;  //numer of points who has been moved to another cluster

  for(unsigned int i = 0; i < Cluster::numberOfPoints; i++) {
    unsigned int source = fits[i];
    arma::rowvec point = points.row(i);

    for(unsigned int k = 0; k < clusters.size(); k++)
      if(k != source) {

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

	  //point moved from cluster source to k  - update fits
	  fits[i] = k;

	  //if cluster has number of members lower than threshold, remove the cluster
	  //threshold is fraction of all points
	  if(clusters[source].size() < killThreshold*Cluster::numberOfPoints) {

	    //delete cluster
	    clusters.erase(clusters.begin() + source);

	    //assign points of erased cluster
	    for(unsigned int j = 0; j < Cluster::numberOfPoints; j++) {

	      //find point of deleted cluster
	      if(fits[j] == source) {
	
		arma::rowvec pointToAssign = points.row(j);
		int minEntropyChangeElementIndex = -1;
		Cluster minEntropyChangeCluster;
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
		      minEntropyChangeCluster = newTarget;						
		    }
		  }
		}
		
		//assert(minEntropyChangeElementIndex > -1);
		clusters[minEntropyChangeElementIndex] = minEntropyChangeCluster;
		fits[j] = minEntropyChangeElementIndex;

	      } else if(fits[j] > source) fits[j]--;
	      //number of clusters is expected to be small in comparison to number
	      //of data points. When you remove a cluster you decrease fits of all
	      //points belonging to clusters with higher position in vector, in order
	      //to keep fits adequate.
	    }			  
	  }

	  break; //point was switched so we'll stop the clusters loop and we'll check the next point
	}
      }  //for iterates clusters
  }  //for iterates points

  return switched;
}

double Hartigan::entropy() {
  return 0;
}
