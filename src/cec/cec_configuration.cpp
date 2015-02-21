#include "cec_configuration.hpp"
#include "const.hpp"
#include <boost/foreach.hpp>
#include <sstream>
#include <string.h>
#include <iostream>

using namespace gmum;



Params CecConfiguration::get_params()
{
    return this->m_params;
}

void CecConfiguration::set_params(Params params)
{
    this->m_params = params;
}

#ifdef RCPP_INTERFACE
void CecConfiguration::set_data_set(const Rcpp::NumericMatrix proxy_dataset)
{
    m_params.dataset = boost::shared_ptr<arma::mat>(new arma::mat(proxy_dataset.begin(), proxy_dataset.nrow(),
                                                                  proxy_dataset.ncol(), false));
}
void CecConfiguration::set_cov(const Rcpp::NumericMatrix cov_mat_proxy)
{
    //TODO better check is empty matrix
    if (!Rf_isNull(cov_mat_proxy) && m_params.clusters.empty())
    {
        m_params.cov_mat_set = true;
        m_params.cov_mat = arma::mat(cov_mat_proxy.begin(), cov_mat_proxy.nrow(),
                                     cov_mat_proxy.ncol());
    } else {
        m_params.cov_mat_set = false;
    }
}

void CecConfiguration::set_mix_handle_full_cluster(Rcpp::List &list)
{
    if (list.containsElementNamed(CONST::CLUSTERS::cov_mat))
    {
        Rcpp::NumericMatrix temp = Rcpp::as < Rcpp::NumericMatrix > (list[CONST::CLUSTERS::cov_mat]);
        m_params.clusters.push_back(boost::make_shared<ClusterFullParams>(arma::mat(temp.begin(), temp.nrow(), temp.ncol())));
    } else {
        GMUM_ERROR(CONST::ERRORS::cov_mat_req);
    }
}

void CecConfiguration::set_mix_handle_fsphere_cluster(Rcpp::List &list)
{
    if (list.containsElementNamed(CONST::CLUSTERS::radius))
    {
        double radius = Rcpp::as<double>(list[CONST::CLUSTERS::radius]);
        m_params.clusters.push_back(boost::make_shared<ClusterFsphereParams>(radius));
    } else {
        m_params.clusters.push_back(boost::make_shared<ClusterFsphereParams>());
    }
}

void CecConfiguration::set_mix_handle_custom_cluster(Rcpp::List &list)
{
    if (list.containsElementNamed(CONST::CLUSTERS::function_name))
    {
        Rcpp::Function func = Rcpp::as<Rcpp::Function>(list[CONST::CLUSTERS::function_name]);
        m_params.clusters.push_back(boost::make_shared<ClusterCustomParams>(boost::make_shared<Rcpp::Function>(func)));
    } else {
        GMUM_ERROR(CONST::ERRORS::function_name_req);
    }
}

void CecConfiguration::set_mix_handle_standard_cluster(Rcpp::List &list)
{
    m_params.clusters.push_back(boost::make_shared<ClusterParams>(kstandard));
}

void CecConfiguration::set_mix_handle_sphere_cluster(Rcpp::List &list)
{
    m_params.clusters.push_back(boost::make_shared<ClusterParams>(ksphere));
}

void CecConfiguration::set_mix_handle_diagonal_cluster(Rcpp::List &list)
{
    m_params.clusters.push_back(boost::make_shared<ClusterParams>(kdiagonal));
}

void CecConfiguration::set_mix(const Rcpp::List clusters)
{
    if (Rf_isNull(clusters)) { return; }

    Rcpp::List desc = Rcpp::as < Rcpp::List > (clusters);
    for (Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
    {
        Rcpp::List list = Rcpp::as < Rcpp::List > (*it);
        if (!list.containsElementNamed(CONST::CLUSTERS::type))
        {
            GMUM_ERROR("method.type not defined");
        }

        std::string typeStr = Rcpp::as < std::string > (list[CONST::CLUSTERS::type]);

        if (typeStr.compare(CONST::CLUSTERS::standard) == 0)
        {
            set_mix_handle_standard_cluster(list);
        } else if (typeStr.compare(CONST::CLUSTERS::full) == 0) {
            set_mix_handle_full_cluster(list);
        } else if (typeStr.compare(CONST::CLUSTERS::fsphere) == 0) {
            set_mix_handle_fsphere_cluster(list);
        } else if (typeStr.compare(CONST::CLUSTERS::sphere) == 0) {
            set_mix_handle_sphere_cluster(list);
        } else if (typeStr.compare(CONST::CLUSTERS::diagonal) == 0) {
            set_mix_handle_diagonal_cluster(list);
        } else if (typeStr.compare(CONST::CLUSTERS::custom) == 0) {
            set_mix_handle_custom_cluster(list);
        } else {
            GMUM_ERROR(typeStr + ": unknown method.type");
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
        GMUM_ERROR(CONST::ERRORS::dataset_size);
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
        GMUM_ERROR(CONST::ERRORS::assignment_error);

    if (m_params.assignment_type == kcentroids
            && m_params.centroids.size() != m_params.nclusters)
        GMUM_ERROR(CONST::ERRORS::centroids_error);

    switch (m_params.cluster_type) {
        case kfull:
        {
            if (!m_params.cov_mat_set) GMUM_ERROR(CONST::ERRORS::cov_mat_req);
            break;
        }
        case kfsphere:
        {
            if (!m_params.radius_set) GMUM_ERROR(CONST::ERRORS::radius_req);
            break;
        }
        case kno_type:
        {
            GMUM_ERROR(CONST::ERRORS::cluster_rec_error);
            break;
        }
#ifdef RCPP_INTERFACE
        case kcustom:
        {
            if (!m_params.function) GMUM_ERROR(CONST::ERRORS::function_name_req);
            break;
        }
#endif
        case kmix:
        {
            BOOST_FOREACH(boost::shared_ptr < ClusterParams > cluster,
                          m_params.clusters)
            {
                switch (cluster->type) {
                case kfull:
                {
                    ClusterFullParams &ptr =
                            static_cast<ClusterFullParams&>(*cluster);
                    if (!ptr.cov_mat_set)
                        GMUM_ERROR(CONST::ERRORS::cov_mat_req);
                    break;
                }
                case kfsphere:
                {
                    ClusterFsphereParams &ptr =
                            static_cast<ClusterFsphereParams&>(*cluster);
                    if (!ptr.radius_set)
                        GMUM_ERROR(CONST::ERRORS::radius_req);
                    break;
                }
                case kno_type:
                {
                    GMUM_ERROR(CONST::ERRORS::cluster_rec_error);
                    break;
                }
#ifdef RCPP_INTERFACE
                case kcustom:
                {
                    ClusterCustomParams &ptr =
                            static_cast<ClusterCustomParams&>(*cluster);
                    if (!ptr.function)
                        GMUM_ERROR(CONST::ERRORS::function_name_req);
                    break;
                }
#endif
                default:
                    break;
                }
            }
            break;
        }
    default:
        break;
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
            GMUM_ERROR(CONST::ERRORS::cluster_rec_error);
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

#ifdef RCPP_INTERFACE
void CecConfiguration::set_function(Rcpp::Function function) {
    if (m_params.clusters.empty()) {
        m_params.function = boost::make_shared<Rcpp::Function>(function);
    }
}
#endif

void CecConfiguration::set_it_max(int it_max) {
    m_params.it_max = it_max;
}
