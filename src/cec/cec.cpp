#include "cec.hpp"
#include "hartigan.hpp"
#include "assignment.hpp"
#include "kmeanspp_assignment.hpp"
#include "centroids_assignment.hpp"
#include "random_assignment.hpp"

using namespace gmum;

boost::shared_ptr<Cluster> CecModel::create_cluster(const ClusterParams &params,
                                                    int i) {
    boost::shared_ptr<Cluster> cluster;
    switch (params.type) {
    case kno_type: // TODO: handle knoType parameter
    case kmix: // TODO: handle kmix parameter
        break;
    case kstandard:
        cluster = boost::shared_ptr < Cluster
                > (new ClusterStandard(i, *m_assignment, *m_points));
        break;
    case kfull: {
        const ClusterFullParams &ptr =
                static_cast<const ClusterFullParams&>(params);
        cluster = boost::shared_ptr < Cluster
                > (new ClusterCovMat(ptr.cov_mat, i, *m_assignment, *m_points));
        break;
    }
    case kdiagonal:
        cluster = boost::shared_ptr < Cluster
                > (new ClusterDiagonal(i, *m_assignment, *m_points));
        break;
    case ksphere:
        cluster = boost::shared_ptr < Cluster
                > (new ClusterSpherical(i, *m_assignment, *m_points));
        break;
    case kfsphere: {
        const ClusterFsphereParams &ptr =
                static_cast<const ClusterFsphereParams&>(params);
        cluster = boost::shared_ptr < Cluster
                > (new ClusterConstRadius(ptr.radius, i, *m_assignment, *m_points));
        break;
    }
    case kcustom: {
        const ClusterCustomParams &ptr =
                static_cast<const ClusterCustomParams&>(params);
        cluster = boost::shared_ptr < Cluster
                > (new ClusterCustomFunction(i, *m_assignment, *m_points,
                                             ptr.function_name));
        break;
    }
    }
    return cluster;
}
CecModel::CecModel(CecConfiguration *cfg) :
    m_config(*cfg) {
    find_best_cec();
}
CecModel::CecModel(const CecModel &other) {
    *this = other;
}

CecModel& CecModel::operator=(const CecModel& other) {
    m_result = other.m_result;
    m_assignment = boost::shared_ptr < std::vector<unsigned int>
            > (new std::vector<unsigned int>(*other.m_assignment));
    m_points = other.m_points;
    m_algorithm = other.m_algorithm;
    m_kill_threshold = other.m_kill_threshold;
    m_inv_set = other.m_inv_set;
    m_inv = other.m_inv;
    m_config = other.m_config;
    m_clusters = other.m_clusters;
    return *this;
}

void CecModel::init(boost::shared_ptr<Algorithm> algorithm,
                    boost::shared_ptr<std::vector<unsigned int> > assignment) {
    Params params = m_config.get_params();
    this->m_assignment = assignment;
    this->m_points = params.dataset;
    this->m_algorithm = algorithm;
    this->m_kill_threshold = params.kill_threshold;
    m_clusters.clear();
    m_clusters.reserve(params.nclusters);

    int i = 0;
    if (params.cluster_type == kmix) {
        BOOST_FOREACH(boost::shared_ptr < ClusterParams > cluster,
                      params.clusters)
        {
            m_clusters.push_back(create_cluster(*cluster, i));
        }
    } else {
        ClusterParams *cluster;
        switch (params.cluster_type) {
        case kfsphere: {
            ClusterFsphereParams *proxy = new ClusterFsphereParams();
            proxy->radius = params.radius;
            cluster = proxy;
            break;
        }
        case kfull: {
            ClusterFullParams *proxy = new ClusterFullParams();
            proxy->cov_mat = params.cov_mat;
            cluster = proxy;

            break;
        }
        case kcustom: {
            ClusterCustomParams *proxy = new ClusterCustomParams();
            proxy->function_name = params.function_name;
            cluster = proxy;
            break;
        }
        default:
            /*case standard:
             case diagonal:
             case sphere:*/
            cluster = new ClusterParams();
            break;
        }
        cluster->type = params.cluster_type;
        for (unsigned int i = 0; i < params.nclusters; ++i)
            m_clusters.push_back(create_cluster(*cluster, i));
        delete cluster;
    }
}

void CecModel::find_best_cec() {
    boost::shared_ptr < std::vector<unsigned int>
            > assignment(new std::vector<unsigned int>());
    boost::shared_ptr<Hartigan> hartigan(
                new Hartigan(this->m_config.get_params().log_nclusters,
                             this->m_config.get_params().log_energy));

    Assignment *assignment_type = NULL;
    switch (this->m_config.get_params().assignment_type) {
    case krandom:
        assignment_type = new RandomAssignment(*(m_config.get_params().dataset),
                                               m_config.get_params().nclusters);
        break;
    case kkmeanspp:
        assignment_type = new KmeansppAssignment(*(m_config.get_params().dataset),
                                                 m_config.get_params().nclusters);
        break;
    case kcentroids:
        assignment_type = new CentroidsAssignment(*(m_config.get_params().dataset),
                                                  m_config.get_params().centroids);
        break;
    case kno_assignment:
        break; // TODO: handle no assignment
    }

    assignment->resize(m_config.get_params().dataset->n_rows);
    (*assignment_type)(*assignment);

    init(hartigan, assignment);

    try {
        loop();
        CecModel best_cec = *this;
        gmum::Params params = m_config.get_params();
        for (unsigned int i = 1; i < params.nstart; ++i) {
            (*assignment_type)(*assignment);
            init(hartigan, assignment);
            loop();

            if (m_result.min_energy < best_cec.get_result().min_energy) {
                best_cec = *this;
            }
        }
        *this = best_cec;
    } catch (std::exception &e) {
        Rcpp::stop(std::string("exception ") + e.what() + " caught in CEC_new");
    }
    delete assignment_type;
}

void CecModel::loop() {
    m_result = m_algorithm->loop(*m_points, *m_assignment, m_kill_threshold, m_clusters);
    int nclusters = m_clusters.size();
    m_inv_set.resize(nclusters, false);
    m_inv.resize(nclusters);
}

void CecModel::single_loop() {
    m_algorithm->single_loop(*m_points, *m_assignment, m_kill_threshold, m_clusters);
}

double CecModel::entropy() {
    double s = 0.0;

    BOOST_FOREACH(boost::shared_ptr < Cluster > cluster, m_clusters)
    {
        s += cluster->entropy();
    }
    return s;
}

std::vector<unsigned int> &CecModel::get_assignment() const {
    return *m_assignment;
}

void CecModel::set_assignment(std::vector<unsigned int> assignment) {
    //TODO set assignment
}

arma::mat CecModel::get_points() {
    return *m_points;
}

std::vector<arma::rowvec> CecModel::centers() const {
    std::vector < arma::rowvec > array;
    array.reserve(m_clusters.size());
    for (unsigned int i = 0; i < m_clusters.size(); ++i) {
        array.push_back(m_clusters[i]->get_mean());
    }
    return array;
}

std::vector<arma::mat> CecModel::cov() const {
    std::vector < arma::mat > array;
    array.reserve(m_clusters.size());

    for (unsigned int i = 0; i < m_clusters.size(); ++i) {
        array.push_back(m_clusters[i]->get_cov_mat(i, *m_assignment, *m_points));
    }

    return array;
}

unsigned int CecModel::iters() const {
    return m_result.iterations;
}

std::list<unsigned int> CecModel::get_nclusters() const {
    return m_result.nclusters;
}

std::list<double> CecModel::get_energy() const {
    return m_result.energy;
}

unsigned int CecModel::predict(std::vector<double> vec) const {
    arma::rowvec x = arma::conv_to < arma::rowvec > ::from(vec);

    int assign = 0;
    double min_entropy_change = std::numeric_limits<double>::max();

    for (unsigned int i = 0; i < m_clusters.size(); ++i) {

        boost::shared_ptr<Cluster> old_cluster = m_clusters[i];
        boost::shared_ptr<Cluster> new_cluster = m_clusters[i]->add_point(x);
        double entropy_change = new_cluster->entropy() - old_cluster->entropy();

        if (entropy_change < min_entropy_change) {
            min_entropy_change = entropy_change;
            assign = i;
        }
    }

    return assign;
}

std::list<double> CecModel::predict(std::vector<double> vec, bool general) {
    arma::rowvec x = arma::conv_to < arma::rowvec > ::from(vec);
    std::list<double> out;

    if (general)
        for (unsigned int i = 0; i < m_clusters.size(); ++i) {
            arma::mat cov_mat = m_clusters[i]->get_cov_mat(i, *m_assignment, *m_points);
            arma::rowvec mean = m_clusters[i]->get_mean();
            if (!m_inv_set[i]) {
                arma::mat Q, R;
                arma::qr_econ(Q, R, cov_mat);
                int dim = mean.n_cols;
                arma::mat Id = arma::eye(dim, dim);
                m_inv[i] = solve(R, Id) * Q.t();
                m_inv_set[i] = true;
            }

            double const_multiplier = sqrt(
                        1.0 / (pow(2 * M_PI, x.n_cols) * arma::det(cov_mat)));
            double scalar = arma::as_scalar(
                        (x - mean) * m_inv[i] * ((x - mean).t()));
            double exponens = exp(-0.5 * scalar);

            out.push_back(const_multiplier * exponens);
        }

    return out;
}

const gmum::TotalResult& CecModel::get_result() const
{
    return m_result;
}

const std::vector<boost::shared_ptr<gmum::Cluster> >& CecModel::get_clusters() const
{
    return m_clusters;
}
