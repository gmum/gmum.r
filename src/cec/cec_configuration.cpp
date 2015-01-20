#include "cec_configuration.hpp"
#include "const.hpp"
#include <boost/foreach.hpp>
#include <sstream>
#include <string.h>
#include <iostream>


using namespace gmum;

CecConfiguration::CecConfiguration() {
}

Params CecConfiguration::get_params() {
    return this->m_params;
}

void CecConfiguration::set_params(Params params) {
    this->m_params = params;
}

#ifdef RCPP_INTERFACE
void CecConfiguration::set_data_set(const Rcpp::NumericMatrix proxy_dataset) {
    //reuses memory and avoids extra copy
    boost::shared_ptr<const arma::mat> points(
                new arma::mat(proxy_dataset.begin(), proxy_dataset.nrow(),
                              proxy_dataset.ncol(), false));
    m_params.dataset = points;
}
void CecConfiguration::set_cov(const Rcpp::NumericMatrix cov_mat_proxy) {
    //TODO better check is empty matrix
    if (!Rf_isNull(cov_mat_proxy) && m_params.clusters.empty()) {
        m_params.cov_mat_set = true;
        m_params.cov_mat = arma::mat(cov_mat_proxy.begin(), cov_mat_proxy.nrow(),
                                     cov_mat_proxy.ncol());
    } else
        m_params.cov_mat_set = false;
}

void CecConfiguration::set_mix(const Rcpp::List clusters) {
    if (!Rf_isNull(clusters)) {
        Rcpp::List desc = Rcpp::as < Rcpp::List > (clusters);
        for (Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it) {
            Rcpp::List list = Rcpp::as < Rcpp::List > (*it);
            boost::shared_ptr<ClusterParams> cluster;
            if (!list.containsElementNamed(CONST::CLUSTERS::type)) {
                cluster.reset(new ClusterParams());
                cluster->type = kno_type;
            } else {
                std::string typeStr = Rcpp::as < std::string
                        > (list[CONST::CLUSTERS::type]);
                if (typeStr.compare(CONST::CLUSTERS::standard) == 0) {
                    cluster.reset(new ClusterParams());
                    cluster->type = kstandard;
                } else if (typeStr.compare(CONST::CLUSTERS::full) == 0) {
                    ClusterFullParams *ptr = new ClusterFullParams();
                    ptr->type = kfull;
                    if (list.containsElementNamed(CONST::CLUSTERS::cov_mat)) {
                        ptr->cov_mat_set = true;
                        Rcpp::NumericMatrix temp = Rcpp::as
                                < Rcpp::NumericMatrix
                                > (list[CONST::CLUSTERS::cov_mat]);
                        ptr->cov_mat = arma::mat(temp.begin(), temp.nrow(),
                                                 temp.ncol());
                    } else
                        ptr->cov_mat_set = false;
                    cluster.reset(ptr);
                } else if (typeStr.compare(CONST::CLUSTERS::fsphere) == 0) {
                    ClusterFsphereParams *ptr = new ClusterFsphereParams();
                    ptr->type = kfsphere;
                    if (list.containsElementNamed(CONST::CLUSTERS::radius)) {
                        ptr->radius_set = true;
                        ptr->radius = Rcpp::as<double>(
                                    list[CONST::CLUSTERS::radius]);
                    } else
                        ptr->radius_set = false;
                    cluster.reset(ptr);
                } else if (typeStr.compare(CONST::CLUSTERS::sphere) == 0) {
                    cluster.reset(new ClusterParams());
                    cluster->type = ksphere;
                } else if (typeStr.compare(CONST::CLUSTERS::diagonal) == 0) {
                    cluster.reset(new ClusterParams());
                    cluster->type = kdiagonal;
                } else if (typeStr.compare(CONST::CLUSTERS::custom) == 0) {
                    ClusterCustomParams *ptr = new ClusterCustomParams();
                    ptr->type = kcustom;
                    if (list.containsElementNamed(CONST::CLUSTERS::function_name)
                            == 0) {
                        ptr->function_name_set = true;
                        ptr->function_name = Rcpp::as < std::string
                                > (list[CONST::CLUSTERS::function_name]);
                    } else {
                        ptr->function_name_set = false;
                    }
                    cluster.reset(ptr);
                } else {
                    cluster.reset(new ClusterParams());
                    cluster->type = kno_type;
                }
            }

            m_params.clusters.push_back(cluster);
        }
    }
}
void CecConfiguration::set_centroids(const Rcpp::List centroids) {
    if (!Rf_isNull(centroids)) {
        Rcpp::List desc = Rcpp::as < Rcpp::List > (centroids);
        for (Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
            m_params.centroids.push_back(Rcpp::as < std::vector<double> > (*it));
        m_params.centroids_set = true;
    } else
        m_params.centroids_set = false;
}
#endif

void CecConfiguration::set_eps(const double kill_threshold) {
    m_params.kill_threshold = kill_threshold;
}

void CecConfiguration::set_nclusters(const unsigned int nclusters) {
    if (nclusters != 0)
        m_params.nclusters = nclusters;
    else if (m_params.clusters.size() > 0)
        m_params.nclusters = m_params.clusters.size();
    else
        m_params.nclusters = CONST::nclusters_init;

    if (m_params.dataset->n_rows < m_params.nclusters)
        ERROR(CONST::ERRORS::dataset_size);
}

void CecConfiguration::set_log_energy(bool log_energy) {
    m_params.log_energy = log_energy;
}

void CecConfiguration::set_log_cluster(bool log_nclusters) {
    m_params.log_nclusters = log_nclusters;
}

void CecConfiguration::set_nstart(const unsigned int nstart) {
    m_params.nstart = nstart;
}



void CecConfiguration::set_method_init(const std::string init) {
    m_params.assignment_type = CONST::default_assignment;
    if (init.compare(CONST::CLUSTERS::random) == 0)
        m_params.assignment_type = krandom;
    else if (init.compare(CONST::CLUSTERS::kmeanspp) == 0)
        m_params.assignment_type = kkmeanspp;
    else if (init.compare(CONST::CLUSTERS::centroids) == 0)
        m_params.assignment_type = kcentroids;
    else
		ERROR(CONST::ERRORS::assignment_error);

    if (m_params.assignment_type == kcentroids
            && m_params.centroids.size() != m_params.nclusters)
        ERROR(CONST::ERRORS::centroids_error);
    switch (m_params.cluster_type) {
    case kstandard: // TODO: handle kstandard parameter
    case kdiagonal: // TODO: handle kdiagonal parameter
    case ksphere: // TODO: handle ksphere parameter
        break;
    case kfull:
        if (!m_params.cov_mat_set)
            ERROR(CONST::ERRORS::cov_mat_req);
        break;
    case kfsphere:
        if (!m_params.radius_set)
            ERROR(CONST::ERRORS::radius_req);
        break;
    case kno_type:
        ERROR(CONST::ERRORS::cluster_rec_error);
        break;
    case kcustom:
        if (!m_params.function_name_set)
            ERROR(CONST::ERRORS::function_name_req);
        break;
    case kmix:
        BOOST_FOREACH(boost::shared_ptr < ClusterParams > cluster,
                      m_params.clusters)
        {
            switch (cluster->type) {
            case kstandard:
            case kdiagonal:
            case ksphere:
            case kmix:
                break;
            case kfull: {
                ClusterFullParams &ptr =
                        static_cast<ClusterFullParams&>(*cluster);
                if (!ptr.cov_mat_set)
                    ERROR(CONST::ERRORS::cov_mat_req);
                break;
            }
            case kfsphere: {
                ClusterFsphereParams &ptr =
                        static_cast<ClusterFsphereParams&>(*cluster);
                if (!ptr.radius_set)
                    ERROR(CONST::ERRORS::radius_req);
                break;
            }
            case kno_type:
                ERROR(CONST::ERRORS::cluster_rec_error);
                break;
            case kcustom:
                ClusterCustomParams &ptr =
                        static_cast<ClusterCustomParams&>(*cluster);
                if (!ptr.function_name_set)
                    ERROR(CONST::ERRORS::function_name_req);
                break;
            }
        }
    }
}

void CecConfiguration::set_method_type(const std::string type) {
    if (m_params.clusters.size() > 0)
        m_params.cluster_type = kmix;
    else {
        if (type.compare(CONST::CLUSTERS::standard) == 0) {            
			m_params.cluster_type = kstandard;
        } else if (type.compare(CONST::CLUSTERS::full) == 0) {
            m_params.cluster_type = kfull;
        } else if (type.compare(CONST::CLUSTERS::diagonal) == 0) {
            m_params.cluster_type = kdiagonal;
        } else if (type.compare(CONST::CLUSTERS::sphere) == 0) {
            m_params.cluster_type = ksphere;
        } else if (type.compare(CONST::CLUSTERS::fsphere) == 0) {
            m_params.cluster_type = kfsphere;
        } else if (type.compare(CONST::CLUSTERS::custom) == 0) {
            m_params.cluster_type = kcustom;
        } else{
        	ERROR(CONST::ERRORS::cluster_rec_error);
        }
    }
}


void CecConfiguration::set_r(const double radius) {
    if (radius != 0 && m_params.clusters.empty()) {
        m_params.radius_set = true;
        m_params.radius = radius;
    } else
        m_params.radius_set = false;
}

void CecConfiguration::set_function(const std::string function_name) {
    if (!function_name.empty() && m_params.clusters.empty()) {
        m_params.function_name_set = true;
        m_params.function_name = function_name;
    } else {
        m_params.function_name_set = false;
    }
}

void CecConfiguration::set_it_max(const unsigned int it_max) {
}

void CecConfiguration::set_iters(bool iters) {
}

