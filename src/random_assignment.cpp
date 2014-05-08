#include "random_assignment.hpp"
#include <time.h>
#include <stdlib.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

void initAssignRandom(std::vector<unsigned int> &assignment,
		      unsigned int nrOfPoints,
		      unsigned int nrOfClusters) {
  int seed = time(NULL);
  boost::random::mt19937 gen(seed);
  boost::random::uniform_int_distribution<> dist(0,nrOfClusters-1);
  assignment.reserve(nrOfPoints);

  for(unsigned int i = 0; i < nrOfPoints; i++)
    assignment.push_back(dist(gen));
}
