#ifndef CLUSTERREADER_HPP
#define CLUSTERREADER_HPP

#include <armadillo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class ClusterReader {
protected:
    std::string prefix();
    std::string input_path();
    std::string cluster_path();
    std::string energy_path();
    std::string dimension_path();
    void normalize_clustering();

    std::string m_folder_name;
    std::vector<std::vector<double> > m_points;
    std::vector<unsigned int> m_clustering;
    double m_energy;
    unsigned int m_dim;
public:
    ClusterReader(const char * name, unsigned int dim);
    ClusterReader(const char * name);
    void read_points();
    void read_clustering();
    void read_dimension();

    void get_points(std::vector<std::vector<double> > & out);
    void get_clustering(std::vector<unsigned int> & out);
    void read_energy();
    double get_energy();
    double get_dimension();
    arma::mat get_points_in_matrix();

};

#endif
