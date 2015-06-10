#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <list>
#include "cluster.hpp"

#ifdef SWIG
%{
#define SWIG_FILE_WITH_INIT
#include "algorithm.hpp"
using namespace gmum;
%}
#endif

namespace gmum {

struct SingleResult {
	int switched;
	int nclusters;
	double energy;
	SingleResult();
	SingleResult(int switched, int nclusters, double energy);
};

struct TotalResult {
	int iterations;
    std::vector<unsigned int> nclusters;
	// energy from all iterations of algorithm
    std::vector<double> energy_history;
	double energy;

	TotalResult();
	void append(SingleResult result, bool log_nlusters, bool log_energy);
};

class Algorithm {
protected:
	bool m_log_nclusters, m_log_energy;
	int m_max_iter;
public:
	Algorithm(bool log_nclusters, bool log_energy, int max_iter); 
	virtual TotalResult loop(const arma::mat &points,
			std::vector<unsigned int> &assignment, double kill_threshold,
			std::vector<Cluster*> &clusters)=0;
	virtual SingleResult single_loop(const arma::mat &points,
			std::vector<unsigned int> &assignment, double kill_threshold,
			std::vector<Cluster*> &clusters)=0;
	virtual ~Algorithm();

};

}

#endif
