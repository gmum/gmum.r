#ifndef CONST_HPP
#define CONST_HPP

#include "params.hpp"

#ifdef SWIG
%{
#define SWIG_FILE_WITH_INIT
#include "const.hpp"
using namespace gmum;
%}
#endif

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
		static const char* full;
		static const char* diagonal;
		static const char* sphere;
		static const char* fsphere;
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

}

#endif
