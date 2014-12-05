#ifndef CLUSTERINGCOMPARATOR_HPP
#define CLUSTERINGCOMPARATOR_HPP

#include <RcppArmadillo.h>
#include <cassert>

class ClusteringComparator {
public:
	virtual std::vector<unsigned int> computePermutation(
			unsigned int numberOfClusters, arma::mat & points,
			std::vector<unsigned int> & realFits,
			std::vector<unsigned int> & myFits) = 0;

	double correct(std::vector<unsigned int> f,
			std::vector<unsigned int> & realFits,
			std::vector<unsigned int> & myFits, unsigned int numberOfElements);

	double evaluateClustering(unsigned int numberOfClusters, arma::mat & points,
			std::vector<unsigned int> & realFits,
			std::vector<unsigned int> & myFits);
};

class BestPermutationComparator: public ClusteringComparator {
public:
	virtual std::vector<unsigned int> computePermutation(
			unsigned int numberOfClusters, arma::mat & points,
			std::vector<unsigned int> & realFits,
			std::vector<unsigned int> & myFits);
};
#endif
