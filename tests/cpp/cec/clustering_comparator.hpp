#ifndef CLUSTERINGCOMPARATOR_HPP
#define CLUSTERINGCOMPARATOR_HPP

#include <RcppArmadillo.h>
#include <cassert>

class ClusteringComparator {
public:
    virtual std::vector<unsigned int> compute_permutation(
            unsigned int nclusters, arma::mat & points,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits) = 0;

	double correct(std::vector<unsigned int> f,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits, unsigned int nelements);

    double evaluate_clustering(unsigned int nclusters, arma::mat & points,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits);
};

class BestPermutationComparator: public ClusteringComparator {
public:
    virtual std::vector<unsigned int> compute_permutation(
            unsigned int nclusters, arma::mat & points,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits);
};
#endif
