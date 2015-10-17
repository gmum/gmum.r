#include "centroids_assignment.hpp"

namespace gmum {

void CentroidsAssignment::operator()(std::vector<unsigned int> &assignment) {
	assign_points(assignment, m_centroids, m_points);
}

}
