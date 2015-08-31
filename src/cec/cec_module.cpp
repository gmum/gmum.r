#ifndef CEC_MODULE_H
#define CEC_MODULE_H

#ifdef RCPP_INTERFACE

#include <RcppArmadillo.h>
using namespace Rcpp;

class CecConfiguration;
class CecModel;

RCPP_EXPOSED_CLASS (CecConfiguration)
RCPP_EXPOSED_CLASS (CecModel)

#include "cec_configuration.hpp"
#include "cec.hpp"
using namespace gmum;

RCPP_MODULE(cec) {
	class_<CecConfiguration>("CecConfiguration")
	.constructor()
	.method("setDataSet", &CecConfiguration::set_data_set)
	.method("setEps", &CecConfiguration::set_eps)
	.method("setMix", &CecConfiguration::set_mix)
	.method("setNrOfClusters", &CecConfiguration::set_nclusters)
	.method("setLogEnergy", &CecConfiguration::set_log_energy)
	.method("setLogCluster", &CecConfiguration::set_log_cluster)
	.method("setNstart", &CecConfiguration::set_nstart)
	.method("setCentroids", &CecConfiguration::set_centroids)
	.method("setMethodInit", &CecConfiguration::set_method_init)
	.method("setMethodType", &CecConfiguration::set_method_type)
	.method("setCov", &CecConfiguration::set_cov)
	.method("setR", &CecConfiguration::set_r)
	.method("setFunction", &CecConfiguration::set_function)
    .method("setItmax", &CecConfiguration::set_it_max)
    .method("setAlgorithm", &CecConfiguration::set_algorithm)
    .method("setSeed", &CecConfiguration::set_seed);

	class_<CecModel>("CecModel")
	.constructor<CecConfiguration*>()
	.method("runAll", &CecModel::loop)
	.method("runOneIteration", &CecModel::single_loop)
	.method("energy", &CecModel::get_energy)
	.method("clustering", &CecModel::get_assignment)
	.method("centers", &CecModel::centers)
	.method("covMatrix", &CecModel::cov)
	.method("predict", &CecModel::predict)
	.method("log.ncluster", &CecModel::get_nclusters)
	.method("log.energy", &CecModel::get_energy_history)
	.method("log.iters", &CecModel::iters)
	.method("getDataset", &CecModel::get_points);
}

#endif

#endif
