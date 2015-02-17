#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include "boost/foreach.hpp"
#include <armadillo>
#include <list>

namespace gmum {

/**
 * Different methods of initiating assignment inherit from this class.
 * You use operator() for getting new assignment.
 */
class Assignment {
protected:
    const arma::mat &m_points;
    const unsigned int m_nclusters;
public:
    Assignment(const arma::mat &points, const unsigned int nclusters) :
        m_points(points), m_nclusters(nclusters) {
    }

    virtual void operator()(std::vector<unsigned int> &assignment) = 0;
    virtual ~Assignment() {
    }
};

/**
 * @centers are ids of rows in points
 */
unsigned int find_nearest(unsigned int i,
                          const std::vector<unsigned int> &centers, const arma::mat &points);

unsigned int find_nearest(unsigned int i,
                          const std::list<std::vector<double> > &centers,
                          const arma::mat &points);

/**
 * @centers are ids of rows in points
 */
void assign_points(std::vector<unsigned int> &assignment,
                   const std::vector<unsigned int> &centers, const arma::mat &points);

void assign_points(std::vector<unsigned int> &assignment,
                   const std::list<std::vector<double> > &centers,
                   const arma::mat &points);
}

#endif
