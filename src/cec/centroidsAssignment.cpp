#include "centroidsAssignment.hpp"

namespace gmum {

  void CentroidsAssignment::operator() (std::vector<unsigned int> &assignment) {
     assignPoints(assignment, centroids, points);
  }

}
