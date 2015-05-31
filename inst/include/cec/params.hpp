#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <string>
#include <list>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "cluster_params.hpp"
#include "algorithm.hpp"

#ifdef SWIG
%{
#define SWIG_FILE_WITH_INIT
#include "params.hpp"
using namespace gmum;
%}
#endif

namespace gmum {

enum AssignmentType {
	kkmeanspp, krandom, kcentroids
};

struct Params {
public:
    arma::mat dataset;
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
    boost::shared_ptr<gmum::Algorithm> algorithm;

#ifdef RCPP_INTERFACE
	boost::shared_ptr<Rcpp::Function> function;
#endif

    Params();

};

}

#endif
