#include "random_assigment.hpp"
#include <time.h>
#include <stdlib.h>

void initAssignRandom(std::vector<unsigned int> &assignment,
		      unsigned int nrOfPoints,
		      unsigned int nrOfClusters) {
  int seed = time(NULL);
  srand(seed);
  assignment.reserve(nrOfPoints);

  for(unsigned int i = 0; i < nrOfPoints; i++) assignment.push_back(rand()%nrOfClusters);
}
