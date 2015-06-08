#ifndef CEC_HPP
#define CEC_HPP

#include <cmath>
#include <list>
#include <vector>
#include "boost/foreach.hpp"
#include "boost/smart_ptr.hpp"
#include "algorithm.hpp"
#include "cluster.hpp"
#include "cluster_custom_function.hpp"
#include "cluster_params.hpp"
#include "cec_configuration.hpp"
#include "exceptions.hpp"
#include "params.hpp"

#ifdef SWIG

%{
#define SWIG_FILE_WITH_INIT
#include "cec.hpp"
using namespace gmum;
%}

%include "std_string.i"
%include "std_vector.i"
%include "std_list.i"
%include "armanpy.i"
%include <boost_shared_ptr.i>

namespace std 
{
    %template(UnsignedIntVector) vector<unsigned int>;
    %template(DoubleVector) vector<double>;
    %template(DoubleList) list<double>;
    %template(UnsignedIntList) list<unsigned int>;
}


#endif

/**
 * Instance of this class is CEC model object.
 */
class CecModel {
public:
	~CecModel();
    CecModel(CecConfiguration& cfg);
    CecModel(CecConfiguration* cfg);
	CecModel(CecModel& other);
	CecModel& operator=(CecModel& other);

	void loop();
	void single_loop();
	double entropy();
	std::vector<unsigned int> get_assignment() const;
	void set_assignment(std::vector<unsigned int>& assignment);
	arma::mat get_points();
    arma::mat centers() const;
	std::vector<arma::mat> cov() const;
	unsigned int iters() const;
	std::list<unsigned int> get_nclusters() const;
	std::list<double> get_energy_history() const;
	double get_energy() const;
	unsigned int predict(std::vector<double> vec);
	const gmum::TotalResult& get_result() const;
private:
    std::vector<gmum::Cluster*> m_clusters;
    gmum::TotalResult m_result;
    std::vector<unsigned int> m_assignment;
    std::vector<bool> m_inv_set;
    std::vector<arma::mat> m_inv;
    CecConfiguration m_config;

    gmum::Cluster * create_cluster(gmum::ClusterParams* cluster_params, int i);
    void find_best_cec();
    void init_clusters(std::vector<unsigned int>& assignment);
    void clear_clusters();
};

#endif
