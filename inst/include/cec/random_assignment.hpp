#ifndef RANDOMASSIGNMENT_HPP
#define RANDOMASSIGNMENT_HPP

#include <stdlib.h>
#include <time.h>
#include <vector>
#include "assignment.hpp"
#include "boost/random/mersenne_twister.hpp"
#include "boost/random/uniform_int_distribution.hpp"
#include "cec_configuration.hpp"

namespace gmum {

void init_assign_random(std::vector<unsigned int> &assignment,
                        const arma::mat &points, unsigned int nclusters);

class RandomAssignment: public Assignment {
public:
    RandomAssignment(const arma::mat &points, const int nclusters) :
        Assignment::Assignment(points, nclusters) {
    }

    virtual void operator()(std::vector<unsigned int> &assignment);

};

}

#endif

