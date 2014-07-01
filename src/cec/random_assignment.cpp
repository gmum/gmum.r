#include "random_assignment.hpp"

namespace gmum {
	
  void RandomAssignment::operator() (std::vector<unsigned int> &assignment) {

    std::vector<unsigned int> centers;
    centers.reserve(nrOfClusters);

    unsigned int nrOfPoints = assignment.size();

    static int seed = time(NULL);
    static boost::random::mt19937 gen(seed);
    
    boost::random::uniform_int_distribution<> dist(0, nrOfPoints-1);
    
    if(nrOfClusters > nrOfPoints){
      Rcpp::stop("Number of clusters cannot be greater than number of points");
    }
     
    for(unsigned int i = 0; i < nrOfClusters; i++){
      while(true){
	unsigned int center = dist(gen);
	if(std::find(centers.begin(), centers.end(), center) == centers.end()){
	  /* centers does not contain dist(gen) */
	  centers.push_back(center);
	  break;
	}
      }
    }
    
    assignPoints(assignment, centers, points);
  }
  
}
