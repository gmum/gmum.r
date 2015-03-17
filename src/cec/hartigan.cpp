#include <climits>
#include "hartigan.hpp"

#define isnan(x) (x != x)

namespace gmum {

Hartigan::Hartigan(bool log_nclusters, bool log_energy, int max_iter) :
		Algorithm(log_nclusters, log_energy, max_iter) {
}

TotalResult Hartigan::loop(const arma::mat &points,
		std::vector<unsigned int> &assignment, double kill_threshold,
		std::vector<Cluster*> &clusters) {
	TotalResult result;
	SingleResult sr;
	do {
		sr = single_loop(points, assignment, kill_threshold, clusters);
		result.append(sr, m_log_nclusters, m_log_energy);

		if ((m_max_iter > 0) && (m_max_iter <= result.iterations)) {
			// if max iter parameter is enabled and there are already max_iter iterations passed, break
			break;
		}

	} while (sr.switched > 0);
	return result;
}

double Hartigan::calc_energy(double cross_entropy, int points_in_cluster,
		int npoints) {
	double p = 1.0 * points_in_cluster / npoints;
	return p * (cross_entropy - log(p));
}

double Hartigan::calc_energy_change(const Cluster& a, const Cluster &b,
		int npoints) {
	double energy_a = calc_energy(a.entropy(), a.size(), npoints);
	double energy_b = calc_energy(b.entropy(), b.size(), npoints);
	return energy_a - energy_b;
}

SingleResult Hartigan::single_loop(const arma::mat &points,
        std::vector<unsigned int> &assignment, double kill_threshold,
        std::vector<Cluster *> &clusters_raw) {

	int switched = 0;  //number of point which has been moved to another cluster
	int dimension = points.n_cols;
	unsigned int npoints = points.n_rows;

//    LOG(m_logger, LogLevel::INFO, to_string(clusters.size()));

	for (unsigned int i = 0; i < npoints; i++) {
		unsigned int source = assignment[i];
		arma::rowvec point = points.row(i);
		double before_source_energy = calc_energy(
				clusters_raw[source]->entropy(), clusters_raw[source]->size(),
				npoints);

		double source_entropy_change = calc_energy(
				clusters_raw[source]->entropy_after_remove_point(point),
				clusters_raw[source]->size() - 1, npoints)
				- before_source_energy;

		double best_entropy_change = 0;
		int best_cluster = -1;

		for (unsigned int k = 0; k < clusters_raw.size(); k++) {
			if (k != source) {
				double whole_entropy_change;
				try {
					double before_target_energy = calc_energy(
							clusters_raw[k]->entropy(), clusters_raw[k]->size(),
							npoints);

					double target_entropy_change = calc_energy(
							clusters_raw[k]->entropy_after_add_point(point),
							clusters_raw[k]->size() + 1, npoints)
							- before_target_energy;

					whole_entropy_change = target_entropy_change
							+ source_entropy_change;
				} catch (std::exception &e) {
					LOG(m_logger, LogLevel::ERR, "entropy change calculation");
					LOG(m_logger, LogLevel::ERR, dimension);
					throw(e);
					//return SingleResult(switched, clusters.size(), 0);
				}

				if (isnan(best_entropy_change)) {
					LOG(m_logger, LogLevel::ERR, "entropy change is NAN");

				}

				if (whole_entropy_change < best_entropy_change) { //newEntropy < oldEntropy
					best_cluster = k;
					best_entropy_change = whole_entropy_change;
				}
			}  //for iterates clusters
		}  // for clusters
		if (best_cluster != -1) {
			switched++;

			clusters_raw[source]->remove_point(point);
			clusters_raw[best_cluster]->add_point(point);

			//point moved from cluster source to k - update assignment
			assignment[i] = best_cluster;

			try {
				//if cluster has number of members lower than threshold, remove the cluster
				//threshold is fraction of all points
				if (clusters_raw[source]->size()
						< std::max(int(kill_threshold * npoints),
								dimension + 1)) {

					remove_cluster(source, points, assignment, clusters_raw);
				}
			} catch (std::exception &e) {
				//LOG(LogLevel::ERR, e.what());
				//TODO: why not terminating here?                
				LOG(m_logger, LogLevel::ERR, "removeCluster");
				throw(e);
			}

		}
	}  //for iterates points

	double energy = 0;
	for (unsigned int i = 0; i < clusters_raw.size(); ++i) {
		energy += calc_energy(clusters_raw[i]->entropy(),
				clusters_raw[i]->size(), npoints);
	}

	//LOG(m_logger, LogLevel::INFO, energy);

	return SingleResult(switched, clusters_raw.size(), energy);
}

void Hartigan::remove_cluster(unsigned int source, const arma::mat &points,
		std::vector<unsigned int> &assignment,
		std::vector<Cluster *> &clusters) {
	//delete cluster
    std::vector<Cluster *>::iterator it = clusters.begin() + source;
    delete *it;
    clusters.erase(it);

	//assign points of erased cluster
	unsigned int npoints = points.n_rows;
	for (unsigned int j = 0; j < npoints; j++) {

		//find point of deleted cluster
		if (assignment[j] == source) {

			arma::rowvec point_to_assign = points.row(j);
			int min_energy_change_element_index = -1;
			double min_energy_change = std::numeric_limits<double>::max();

			//find the best cluster to assign the point to it
			for (unsigned int k = 0; k < clusters.size(); k++) {

				double before_energy = calc_energy(clusters[k]->entropy(),
						clusters[k]->size(), npoints);

				clusters[k]->add_point(point_to_assign);

				//std::cerr<<clusters[k]->entropy()<<std::endl;

				double energy_change = calc_energy(clusters[k]->entropy(),
						clusters[k]->size(), npoints) - before_energy;

				clusters[k]->remove_point(point_to_assign);

				if (energy_change < min_energy_change) {
					min_energy_change = energy_change;
					min_energy_change_element_index = k;
				}

			}
#ifdef DEBUG
			assert(minEntropyChangeElementIndex > -1);
#endif
			//we are here adding and then removing
			clusters[min_energy_change_element_index]->add_point(
					point_to_assign);
			assignment[j] = min_energy_change_element_index;

		} else if (assignment[j] > source)
			assignment[j]--;
		//number of clusters is expected to be small in comparison to number
		//of data points. When you remove a cluster you decrease assignment of all
		//points belonging to clusters with higher position in vector, in order
		//to keep assignment adequate.
	}
}
double Hartigan::entropy(boost::shared_ptr<Cluster> ptr_to_cluster,
		int npoints) {
	double p = (1.0 * ptr_to_cluster->size()) / npoints;
	return p * ptr_to_cluster->entropy() + p * log(-p);
}
}
