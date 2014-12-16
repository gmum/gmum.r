#include "clustering_comparator.hpp"

double ClusteringComparator::correct(std::vector<unsigned int> f,
        std::vector<unsigned int> & real_fits,
        std::vector<unsigned int> & my_fits, unsigned int nelements){
    int ncorrect = 0;
    for (unsigned int i = 0; i < nelements; ++i)
        ncorrect += (real_fits[i] == f[my_fits[i]]);
    //    std::cout << "numberOfCorrect = " << numberOfCorrect << std::endl;

    return 1.0 * ncorrect / nelements;
}

double ClusteringComparator::evaluate_clustering(unsigned int nclusters, arma::mat & points,
        std::vector<unsigned int> & real_fits,
        std::vector<unsigned int> & my_fits)
{
    std::vector<unsigned int> f = compute_permutation(nclusters,
            points, real_fits, my_fits);
    return correct(f, real_fits, my_fits, points.n_rows);
}


std::vector<unsigned int> BestPermutationComparator::compute_permutation(unsigned int nclusters, arma::mat & points,
        std::vector<unsigned int> & real_fits,
        std::vector<unsigned int> & my_fits)
{
	std::vector<unsigned int> current;
    for (unsigned int i = 0; i < nclusters; ++i)
		current.push_back(i);
	std::vector<unsigned int> best;
    double best_value = 0.0;
	do {
        double current_value = correct(current, real_fits, my_fits, points.n_rows);
		// std::cout << "Current value " << currentValue << std::endl;
        if (best_value < current_value) {
			best = current;
            best_value = current_value;
		}
	} while (std::next_permutation(current.begin(), current.end()));
	return best;
}
