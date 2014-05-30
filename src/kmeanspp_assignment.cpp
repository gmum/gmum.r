#include "kmeanspp_assignment.hpp"

namespace gmum {

  void initAssignKmeanspp(std::vector<unsigned int> &assignment,
			  arma::mat &points,
			  unsigned int nrOfPoints,
			  unsigned int nrOfClusters) {
    
    std::vector<unsigned int> centers;
    centers.reserve(nrOfClusters);

    static int seed = time(NULL);
    static boost::random::mt19937 gen(seed);

    //select points for centers
    std::list<Pair> selected;
    unsigned int nrOfSections = 100;
    unsigned int section = nrOfPoints/nrOfSections;
    if(section <= 1)
      for(unsigned int i=0; i<nrOfPoints; ++i) selected.push_back(i);
    else {
      boost::random::uniform_int_distribution<> uniform(0,section-1);
      unsigned int k=0;
      //nrOfPoints > section
      for(; k<nrOfPoints-section; k+=section) {
	unsigned int target = k+uniform(gen);
	selected.push_back(target);
      }
      k += uniform(gen);
      if(k >= nrOfPoints) k = nrOfPoints - 1;
      selected.push_back(k);
    }

    //first center
    centers.push_back(selected.front().pointNumber);
    selected.pop_front();
    --nrOfClusters;

    //choose other centers
    float bernoulli_p = 0.8;
    boost::random::bernoulli_distribution<> bernoulli(bernoulli_p);

    for(; nrOfClusters > 0; --nrOfClusters) {
      calculateDistance(centers, selected, points);
      std::list<Pair>::iterator it = choose(bernoulli, gen, selected);
      centers.push_back(selected.front().pointNumber);
      selected.pop_front();
    }

    //assign points
    for(unsigned int i=0; i<nrOfPoints; ++i)
      assignment.push_back(findNearest(i, centers, points));

  }

  void calculateDistance(std::vector<unsigned int> &centers,
			 std::list<Pair> &selected,
			 arma::mat &points) {

    for(std::list<Pair>::iterator it=selected.begin(); it != selected.end(); ++it) {
      arma::rowvec point = points.row(it->pointNumber);
      float distance = std::numeric_limits<float>::max();

      for(unsigned int i=0; i<centers.size(); ++i) {
	arma::rowvec vec = points.row(centers[i]) - point;
	float tempDist = arma::as_scalar(vec*vec.t());
	if(distance > tempDist) distance = tempDist;
      }

      it->distance = distance;
    }

    selected.sort();
  }

  std::list<Pair>::iterator choose(boost::random::bernoulli_distribution<> &bernoulli,
			      boost::random::mt19937 &gen,
			      std::list<Pair> &selected) {

    std::list<Pair>::iterator it = selected.begin();
    for(; !bernoulli(gen) && it != selected.end(); ++it);

    if(it == selected.end()) {
      it = selected.end();
      --it;
    }

    return it;
  }

  unsigned int findNearest(unsigned int i, 
			   std::vector<unsigned int> &centers,
			   arma::mat &points) {

    arma::rowvec point = points.row(i);
    float distance = std::numeric_limits<float>::max();
    unsigned int nearest = -1;

    for(unsigned int i=0; i<centers.size(); ++i) {

      arma::rowvec vec = points.row(centers[i]) - point;
      float tempDist = arma::as_scalar(vec*vec.t());

      if(distance > tempDist) {
	distance = tempDist;
	nearest = i;
      }
    }

    return nearest;
  }

}
