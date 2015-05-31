#include "params.hpp"

using namespace gmum;

Params::Params() :
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
