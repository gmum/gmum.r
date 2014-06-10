#include "random_assignment.hpp"

namespace gmum {

  void initAssignRandom(std::vector<unsigned int> &assignment,
			unsigned int nrOfClusters) {

    unsigned int nrOfPoints = assignment.size();

    static int seed = time(NULL);
    static boost::random::mt19937 gen(seed);
    boost::random::uniform_int_distribution<> dist(0,nrOfClusters-1);

    for(unsigned int i = 0; i < nrOfPoints; i++)
      assignment[i] = dist(gen);
  }

}
