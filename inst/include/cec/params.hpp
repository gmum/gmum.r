#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <string>
#include <list>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "cluster_params.hpp"

namespace gmum {

enum AssignmentType {
	kkmeanspp, krandom, kcentroids
};

struct Params {
public:
	boost::shared_ptr<arma::mat> dataset;
	double kill_threshold;
	unsigned int nclusters;
	bool log_nclusters;
	bool log_energy;
	unsigned int nstart;
	int it_max;
	AssignmentType assignment_type;
	bool centroids_set;
	std::list<std::vector<double> > centroids;
	ClusterType cluster_type;
	std::list<boost::shared_ptr<ClusterParams> > clusters;
	bool cov_mat_set;
	arma::mat cov_mat;
	bool radius_set;
	double radius;

#ifdef RCPP_INTERFACE
	boost::shared_ptr<Rcpp::Function> function;
#endif

    Params() :
        kill_threshold(0.0001),
        nclusters(0),
        log_nclusters(false),
        log_energy(false),
        nstart(1),
        it_max(25),
        assignment_type(kkmeanspp),
        centroids_set(false),
        cluster_type(kno_type),
        cov_mat_set(false),
        radius_set(false),
        radius(1.5)
        { }
};

}

#endif
