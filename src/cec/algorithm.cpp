#include "algorithm.hpp"

using namespace gmum;

SingleResult::SingleResult() {
    switched = 0;
    nclusters = 0;
    energy = 0.0;
}

SingleResult::SingleResult(int switched, int nclusters, double energy) :
        switched(switched), nclusters(nclusters), energy(energy) {
}


TotalResult::TotalResult() :
        iterations(0) {
    energy = std::numeric_limits<double>::max();
}

void TotalResult::append(SingleResult result, bool log_nlusters, bool log_energy) {
    ++iterations;
    energy = result.energy;
    if (log_nlusters) {
        nclusters.push_back(result.nclusters);
    }
    if (log_energy) {
        energy_history.push_back(result.energy);
    }
}


Algorithm::Algorithm(bool log_nclusters, bool log_energy, int max_iter) :
        m_log_nclusters(log_nclusters), m_log_energy(log_energy), m_max_iter(
                max_iter) {
}
Algorithm::~Algorithm() {}


