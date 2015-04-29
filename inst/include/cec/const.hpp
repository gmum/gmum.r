#ifndef CONST_HPP
#define CONST_HPP

#include "params.hpp"

namespace gmum {

struct CONST {
	static const char* dataset;
	static const char* nclusters;
	static const char* clusters;
	static const char* nstart;
	static const char* centroids_list;
	static const char* kill_threshold;
	static const char* itmax;
	static const char* energy;
	static const char* log_clusters;

	struct CLUSTERS {
		static const char* type;
		static const char* standard;
		static const char* fixed_covariance;
		static const char* diagonal;
		static const char* spherical;
		static const char* fixed_spherical;
		static const char* custom;

		static const char* init;
		static const char* kmeanspp;
		static const char* random;
		static const char* centroids;

		static const char* cov_mat;
		static const char* radius;
		static const char* function_name;
	};

	static const unsigned int nclusters_init;
	static const AssignmentType default_assignment;

	struct ERRORS {
		static const char* dataset_size;
		static const char* assignment_error;
		static const char* cov_mat_req;
		static const char* radius_req;
		static const char* cluster_rec_error;
		static const char* centroids_error;
		static const char* function_name_req;
	};
};

const char* CONST::dataset = "x";
const char* CONST::nclusters = "k";
const char* CONST::clusters = "params.mix";
const char* CONST::nstart = "control.nstart";
const char* CONST::centroids_list = "params.centroids";
const char* CONST::kill_threshold = "control.eps";
const char* CONST::itmax = "control.itmax";
const char* CONST::energy = "log.energy";
const char* CONST::log_clusters = "log.ncluster";

const char* CONST::CLUSTERS::type = "method.type";
const char* CONST::CLUSTERS::standard = "standard";
const char* CONST::CLUSTERS::fixed_covariance = "fixed_covariance";
const char* CONST::CLUSTERS::diagonal = "diagonal";
const char* CONST::CLUSTERS::spherical = "spherical";
const char* CONST::CLUSTERS::fixed_spherical = "fixed_spherical";
const char* CONST::CLUSTERS::custom = "func";

const char* CONST::CLUSTERS::init = "method.init";
const char* CONST::CLUSTERS::kmeanspp = "kmeans++";
const char* CONST::CLUSTERS::random = "random";
const char* CONST::CLUSTERS::centroids = "centroids";

const char* CONST::CLUSTERS::cov_mat = "params.cov";
const char* CONST::CLUSTERS::radius = "params.r";
const char* CONST::CLUSTERS::function_name = "params.function";

const unsigned int CONST::nclusters_init = 10;
const AssignmentType CONST::default_assignment = kkmeanspp;
const char* CONST::ERRORS::dataset_size =
		"Size of dataset cannot be less than number of clusters!";
const char* CONST::ERRORS::assignment_error =
		"Cannot recognise assignment initialization method!";
const char* CONST::ERRORS::cov_mat_req = "Covariance matrix is required!";
const char* CONST::ERRORS::radius_req = "Radius is required!";
const char* CONST::ERRORS::cluster_rec_error = "Cannot recognise cluster type!";
const char* CONST::ERRORS::centroids_error =
		"Number of centroids is different from number of clusters!";
const char* CONST::ERRORS::function_name_req = "Function is required!";

}

#endif
