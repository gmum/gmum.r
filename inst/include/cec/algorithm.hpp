#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <list>
#include "cluster.hpp"

namespace gmum {

struct SingleResult {
	int switched;
	int nclusters;
	double energy;
	SingleResult() {
		switched = 0;
		nclusters = 0;
		energy = 0.0;
	}
	SingleResult(int switched, int nclusters, double energy) :
			switched(switched), nclusters(nclusters), energy(energy) {
	}
};

struct TotalResult {
	int iterations;
	std::list<unsigned int> nclusters;
	// energy from all iterations of algorithm
    std::list<double> energy_history;
    double energy;

	TotalResult() :
			iterations(0) {
        energy = std::numeric_limits<double>::max();
	}
	void append(SingleResult result, bool log_nlusters, bool log_energy) {
        ++iterations;
        energy = result.energy;
		if (log_nlusters)
        {
			nclusters.push_back(result.nclusters);
        }
		if (log_energy)
        {
            energy_history.push_back(result.energy);
        }
	}
};

class Algorithm {
protected:
    bool m_log_nclusters, m_log_energy;
public:
	Algorithm(bool log_nclusters, bool log_energy) :
			m_log_nclusters(log_nclusters), m_log_energy(log_energy) {
	}
	virtual TotalResult loop(const arma::mat &points,
			std::vector<unsigned int> &assignment, double kill_threshold,
			std::vector<Cluster*> &clusters)=0;
	virtual SingleResult single_loop(const arma::mat &points,
			std::vector<unsigned int> &assignment, double kill_threshold,
			std::vector<Cluster*> &clusters)=0;
	virtual ~Algorithm() {
	}

};

}

#endif
