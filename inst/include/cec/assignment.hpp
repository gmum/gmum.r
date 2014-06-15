#include <RcppArmadillo.h>

#ifndef ASSIGMENT_HPP
#define ASSIGMENT_HPP

namespace gmum {

  unsigned int findNearest(unsigned int i, 
			   const std::vector<unsigned int> &centers,
			   const arma::mat &points);

}

#endif
