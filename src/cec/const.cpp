#include "const.hpp"

using namespace gmum;

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
const char* CONST::CLUSTERS::full = "full";
const char* CONST::CLUSTERS::diagonal = "diagonal";
const char* CONST::CLUSTERS::sphere = "sphere";
const char* CONST::CLUSTERS::fsphere = "fsphere";
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

