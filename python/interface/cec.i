
%module cec
%include "std_string.i"
%{
#define SWIG_FILE_WITH_INIT
#include "cec_configuration.hpp"
%}

class CecConfiguration {
public:
    CecConfiguration();
    ~CecConfiguration();

    gmum::Params& get_params();
    void set_params(gmum::Params params);
    void set_eps(double kill_threshold);
    void set_nclusters(unsigned int nclusters);
    void set_log_energy(bool log_energy);
    void set_log_cluster(bool log_nclusters);
    void set_nstart(unsigned int nstart);
    void set_method_init(std::string init);
    void set_method_type(std::string type);
    void set_r(double radius);
    void set_it_max(int it_max);
    void set_algorithm(std::string algorithm);
private:
    gmum::Params m_params;
};

