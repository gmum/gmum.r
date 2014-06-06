#include "Hartigan.hpp"

namespace gmum {

  Hartigan::Hartigan(bool logNrOfClusters, bool logEnergy) : Algorithm(logNrOfClusters, logEnergy) {}

  TotalResult Hartigan::loop(arma::mat &points, std::vector<unsigned int> &assignment,
			     float killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters) {
    TotalResult result;
    SingleResult sr;

    do {
      sr = singleLoop(points, assignment, killThreshold, clusters);
      result.append(sr, logNrOfClusters, logEnergy);
    } while(sr.switched > 0);

    return result;
  }

  SingleResult Hartigan::singleLoop(arma::mat &points, std::vector<unsigned int> &assignment, 
				    float killThreshold, std::vector<boost::shared_ptr<Cluster> > &clusters) {

    int switched = 0;  //numer of points who has been moved to another cluster
    int dimension = points.n_cols;

    for(unsigned int i = 0; i < Cluster::numberOfPoints; i++) {
      unsigned int source = assignment[i];
      arma::rowvec point = points.row(i);

      for(unsigned int k = 0; k < clusters.size(); k++)
	if(k != source) {

	  boost::shared_ptr<Cluster> oldSource, oldTarget, newSource, newTarget;
	  float oldEntropy, newEntropy;

	  try {

	    oldSource = clusters[source];
	    oldTarget = clusters[k];
	    newSource = clusters[source]->removePoint(point);
	    newTarget = clusters[k]->addPoint(point);
	    oldEntropy = oldSource->entropy()+oldTarget->entropy() ;
	    newEntropy = newSource->entropy()+newTarget->entropy();


	  } catch(std::exception e) {
	    std::cout << "removePoint" << std::endl;
	    std::cout << dimension << std::endl;
	    std::cout << oldSource->size() << std::endl;
	    std::cout << oldTarget->size() << std::endl;
	    throw(e);
	    //return SingleResult(switched, clusters.size(), 0);
	  }

	  if(newEntropy < oldEntropy) {
	    clusters[source] = newSource;
	    clusters[k] = newTarget;
	    switched++;

	    //point moved from cluster source to k - update assignment
	    assignment[i] = k;

	    try {
	      //if cluster has number of members lower than threshold, remove the cluster
	      //threshold is fraction of all points
	      if(clusters[source]->size() < std::max(int(killThreshold*Cluster::numberOfPoints),dimension+1)) {

		removeCluster(source, points, assignment, clusters);
	      }
	    } catch(std::exception e) {
	      //std::cout << e.what() << std::endl;
	      std::cout << "removeCluster" << std::endl;
	      throw(e);
	    }

	    break; //point was switched so we'll stop the clusters loop and we'll check the next point
	  }
	}  //for iterates clusters
    }  //for iterates points

    float energy = 0;
    if(logEnergy)
      for(int i=0; i<clusters.size(); ++i) energy += clusters[i]->entropy();

    return SingleResult(switched, clusters.size(), energy);
  }


  void Hartigan::removeCluster(unsigned int source, arma::mat &points, std::vector<unsigned int> &assignment,
			       std::vector<boost::shared_ptr<Cluster> > &clusters) {
    //delete cluster
    clusters.erase(clusters.begin() + source);

    //assign points of erased cluster
    for(unsigned int j = 0; j < Cluster::numberOfPoints; j++) {

      //find point of deleted cluster
      if(assignment[j] == source) {
	
	arma::rowvec pointToAssign = points.row(j);
	int minEntropyChangeElementIndex = -1;
	boost::shared_ptr<Cluster> minEntropyChangeCluster;
	float minEntropyChange = std::numeric_limits<float>::max();
		
	//find the best cluster to assign the point to it 
	for(unsigned int l = 0; l < clusters.size(); l++){
	  boost::shared_ptr<Cluster> oldTarget = clusters[l];
	  boost::shared_ptr<Cluster> newTarget = clusters[l]->addPoint(pointToAssign);
	  float entropyChange = newTarget->entropy() - oldTarget->entropy();
	  if(entropyChange < minEntropyChange){
	    minEntropyChange = entropyChange;
	    minEntropyChangeElementIndex = l;
	    minEntropyChangeCluster = newTarget;						
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

}
