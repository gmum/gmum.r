#ifndef CEC_HPP
#define CEC_HPP

#include <list>
#include <vector>
#include "boost/foreach.hpp"
#include "boost/smart_ptr.hpp"
#include "boost/interprocess/smart_ptr/unique_ptr.hpp"
#include "algorithm.hpp"
#include "cluster.hpp"
#include "cluster_custom_function.hpp"
#include "cluster_params.hpp"
#include "cec_configuration.hpp"
#include "exceptions.hpp"
#include "params.hpp"

/**
 * Instance of this class is CEC model object.
 */
class CecModel {
private:
    std::vector< gmum::Cluster* > m_clusters;
    gmum::TotalResult m_result;
    std::vector<unsigned int> m_assignment;
    arma::mat m_points;
    boost::shared_ptr<gmum::Algorithm> m_algorithm;
    double m_kill_threshold;
    std::vector<bool> m_inv_set;
    std::vector<arma::mat> m_inv;
    CecConfiguration m_config;

    gmum::Cluster * create_cluster(const gmum::ClusterParams &params,
                                                    int i);
    void find_best_cec();
    void init(boost::shared_ptr<gmum::Algorithm> algorithm, std::vector<unsigned int>& assignment);
public:
    CecModel(CecConfiguration* cfg);
    CecModel(const CecModel& other);
    CecModel& operator=(const CecModel& other);

    void loop();
    void single_loop();
    double entropy();
    std::vector<unsigned int> get_assignment() const;
    void set_assignment(std::vector<unsigned int>& m_assignment);
    arma::mat get_points();
    std::vector<arma::rowvec> centers() const;
    std::vector<arma::mat> cov() const;
    unsigned int iters() const;
    std::list<unsigned int> get_nclusters() const;
    std::list<double> get_energy() const;
    unsigned int predict(std::vector<double> vec) const;
    std::list<double> predict(std::vector<double> vec, bool general);
    const gmum::TotalResult& get_result() const;
};

#endif
