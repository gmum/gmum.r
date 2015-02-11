#ifndef HARTIGAN_HPP
#define HARTIGAN_HPP

#include <cmath>
#include <limits>
#include <vector>
#include "algorithm.hpp"
#include "cluster.hpp"
#include "../utils/log.hpp"
#include <sstream>

namespace gmum {


//TODO: clean 3 loggers (:P) and include utils/utils.h with this function
template<class T>
static std::string to_string(const T& x) {
	std::stringstream ss;
	ss << x;
	return ss.str();
}


class Hartigan: public Algorithm {

	Logger m_logger;
    void remove_cluster(unsigned int source, const arma::mat &points,
                        std::vector<unsigned int> &assignment,
                        std::vector<Cluster* > &clusters);
    double calc_energy(double cross_entropy, int points_in_cluster,
                       int npoints);
    double calc_energy_change(const Cluster& a, const Cluster &b,
                              int npoints);
public:
    Hartigan(bool m_log_nclusters, bool m_log_energy);
    TotalResult loop(const arma::mat &points,
                     std::vector<unsigned int> &assignment, double kill_threshold,
                     std::vector<Cluster* > &clusters);
    SingleResult single_loop(const arma::mat &points,
                             std::vector<unsigned int> &assignment, double kill_threshold,
                             std::vector<Cluster* > &clusters);
    double entropy(boost::shared_ptr<Cluster> ptr_to_cluster, int npoints);
};

}

#endif
