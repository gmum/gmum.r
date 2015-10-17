#ifndef CLUSTERINGCOMPARATOR_HPP
#define CLUSTERINGCOMPARATOR_HPP

#include <armadillo>
#include <cassert>

class ClusteringComparator {
public:
    virtual std::vector<unsigned int> compute_permutation(
            unsigned int nclusters,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits) = 0;

    double correct(std::vector<unsigned int> f,
                   std::vector<unsigned int> & real_fits,
                   std::vector<unsigned int> & my_fits);

    double evaluate_clustering(unsigned int nclusters,
                               std::vector<unsigned int> & real_fits,
                               std::vector<unsigned int> & my_fits);
};

class BestPermutationComparator: public ClusteringComparator {
public:
    virtual std::vector<unsigned int> compute_permutation(
            unsigned int nclusters,
            std::vector<unsigned int> & real_fits,
            std::vector<unsigned int> & my_fits);
};
#endif
