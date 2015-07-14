#include "random_assignment.hpp"

#ifdef RCPP_INTERFACE
#include <Rcpp.h>
#endif

namespace gmum {

void RandomAssignment::operator()(std::vector<unsigned int> &assignment) {

    std::vector<unsigned int> centers;
    centers.reserve(m_nclusters);

    unsigned int npoints = assignment.size();
    boost::random::mt19937 gen(seed);
    boost::random::uniform_int_distribution<> dist(0, npoints - 1);

    if (m_nclusters > npoints) {
        GMUM_ERROR("Number of clusters cannot be greater than number of points");
    }

    for (unsigned int i = 0; i < m_nclusters; i++) {
        while (true) {
            unsigned int center = dist(gen);
			if (std::find(centers.begin(), centers.end(), center)
                    == centers.end()) {
                /* centers does not contain dist(gen) */
                centers.push_back(center);
                break;
            }
        }
    }

    assign_points(assignment, centers, m_points);
}

}
