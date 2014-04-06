#include "Hartigan.hpp"

void Hartigan::loop(arma::mat &points, std::vector<unsigned int> &assignment,
		    float killThreshold, std::vector<Cluster> &clusters) {
  while(singleLoop(points, assignment, killThreshold, clusters) > 0);
}

int Hartigan::singleLoop(arma::mat &points, std::vector<unsigned int> &assignment, 
			 float killThreshold, std::vector<Cluster> &clusters) {

  int switched = 0;  //numer of points who has been moved to another cluster

  for(unsigned int i = 0; i < Cluster::numberOfPoints; i++) {
    unsigned int source = assignment[i];
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

	  //point moved from cluster source to k  - update assignment
	  assignment[i] = k;

	  //if cluster has number of members lower than threshold, remove the cluster
	  //threshold is fraction of all points
	  if(clusters[source].size() < killThreshold*Cluster::numberOfPoints) {

	    //delete cluster
	    clusters.erase(clusters.begin() + source);

	    //assign points of erased cluster
	    for(unsigned int j = 0; j < Cluster::numberOfPoints; j++) {

	      //find point of deleted cluster
	      if(assignment[j] == source) {
	
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
		assignment[j] = minEntropyChangeElementIndex;

	      } else if(assignment[j] > source) assignment[j]--;
	      //number of clusters is expected to be small in comparison to number
	      //of data points. When you remove a cluster you decrease assignment of all
	      //points belonging to clusters with higher position in vector, in order
	      //to keep assignment adequate.
	    }			  
	  }

	  break; //point was switched so we'll stop the clusters loop and we'll check the next point
	}
      }  //for iterates clusters
  }  //for iterates points

  return switched;
}
