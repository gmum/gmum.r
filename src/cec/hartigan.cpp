#include <climits>
#include "hartigan.hpp"

namespace gmum {

Hartigan::Hartigan(bool log_nclusters, bool log_energy) : Algorithm(log_nclusters, log_energy) {
    this->m_logger = Logger();
}

TotalResult Hartigan::loop(const arma::mat &points, std::vector<unsigned int> &assignment,
                           double kill_threshold, std::vector<boost::shared_ptr<Cluster> > &clusters) {
    TotalResult result;
    SingleResult sr;
    double min_energy = std::numeric_limits<double>::max();

    do {
        sr = single_loop(points, assignment, kill_threshold, clusters);
        min_energy = std::min(min_energy, sr.energy);
        result.append(sr, m_log_nclusters, m_log_energy);
    } while(sr.switched > 0);
    result.min_energy = min_energy;
    return result;
}

double Hartigan::calc_energy(double cross_entropy, int points_in_cluster, int npoints) {
    double p = 1.0*points_in_cluster/npoints;
    return p*(cross_entropy-log(p));
}

double Hartigan::calc_energy_change(const Cluster& a, const Cluster &b, int npoints) {
    double entropy_a = calc_energy(a.entropy(), a.size(), npoints);
    double entropy_b = calc_energy(b.entropy(), b.size(), npoints);
    return entropy_a - entropy_b;
}

SingleResult Hartigan::single_loop(const arma::mat &points, std::vector<unsigned int> &assignment,
                                   double kill_threshold, std::vector<boost::shared_ptr<Cluster> > &clusters) {

    int switched = 0;  //number of point which has been moved to another cluster
    int dimension = points.n_cols;
    unsigned int npoints = points.n_rows;
    for(unsigned int i = 0; i < npoints; i++) {
        unsigned int source = assignment[i];
        arma::rowvec point = points.row(i);

        for(unsigned int k = 0; k < clusters.size(); k++)
            if(k != source) {

                boost::shared_ptr<Cluster> old_source, old_target, new_source, new_target;
                double whole_entropy_change;

                try {

                    old_source = clusters[source];
                    old_target = clusters[k];
                    new_source = clusters[source]->remove_point(point);
                    new_target = clusters[k]->add_point(point);

                    double source_entropy_change =
                            calc_energy_change(*new_source, *old_source, npoints);
                    double target_entropy_change =
                            calc_energy_change(*new_target, *old_target, npoints);

                    whole_entropy_change = target_entropy_change+source_entropy_change;

                } catch(std::exception e) {
                    LOG(LogLevel::ERR, "removePoint");
                    LOG(LogLevel::ERR, dimension);
                    LOG(LogLevel::ERR, old_source->size());
                    LOG(LogLevel::ERR, old_target->size());
                    throw(e);
                    //return SingleResult(switched, clusters.size(), 0);
                }

                if(whole_entropy_change < 0) {  //newEntropy < oldEntropy
                    clusters[source] = new_source;
                    clusters[k] = new_target;
                    switched++;

                    //point moved from cluster source to k - update assignment
                    assignment[i] = k;

                    try {
                        //if cluster has number of members lower than threshold, remove the cluster
                        //threshold is fraction of all points
                        if(clusters[source]->size() < std::max(int(kill_threshold*npoints),dimension+1)) {

                            remove_cluster(source, points, assignment, clusters);
                        }
                    } catch(std::exception e) {
                        //LOG(LogLevel::ERR, e.what());
                        LOG(LogLevel::ERR, "removeCluster");
                        throw(e);
                    }

                    break; //point was switched so we'll stop the clusters loop and we'll check the next point
                }
            }  //for iterates clusters
    }  //for iterates points

    double energy = 0;
    for(unsigned int i=0; i<clusters.size(); ++i)
    {
        energy += calc_energy(clusters[i]->entropy(), clusters[i]->size(), npoints);
    }

    return SingleResult(switched, clusters.size(), energy);
}


void Hartigan::remove_cluster(unsigned int source, const arma::mat &points,
                              std::vector<unsigned int> &assignment,
                              std::vector<boost::shared_ptr<Cluster> > &clusters) {
    //delete cluster
    clusters.erase(clusters.begin() + source);

    //assign points of erased cluster
    unsigned int npoints = points.n_rows;
    for(unsigned int j = 0; j < npoints; j++) {

        //find point of deleted cluster
        if(assignment[j] == source) {

            arma::rowvec point_to_assign = points.row(j);
            int min_energy_change_element_index = -1;
            boost::shared_ptr<Cluster> min_energy_change_cluster;
            double min_energy_change = std::numeric_limits<double>::max();

            //find the best cluster to assign the point to it
            for(unsigned int k = 0; k < clusters.size(); k++){
                boost::shared_ptr<Cluster> old_target = clusters[k];
                boost::shared_ptr<Cluster> new_target = clusters[k]->add_point(point_to_assign);

                double energy_change =
                        calc_energy_change(*new_target, *old_target, npoints);

                if(energy_change < min_energy_change){
                    min_energy_change = energy_change;
                    min_energy_change_element_index = k;
                    min_energy_change_cluster = new_target;
                }
            }

            //assert(minEntropyChangeElementIndex > -1);
            clusters[min_energy_change_element_index] = min_energy_change_cluster;
            assignment[j] = min_energy_change_element_index;

        } else if(assignment[j] > source) assignment[j]--;
        //number of clusters is expected to be small in comparison to number
        //of data points. When you remove a cluster you decrease assignment of all
        //points belonging to clusters with higher position in vector, in order
        //to keep assignment adequate.
    }
}
double Hartigan::entropy(boost::shared_ptr<Cluster> ptr_to_cluster,
                         int npoints) {
    double p = (1.0*ptr_to_cluster->size())/npoints;
    return p*ptr_to_cluster->entropy()  + p*log(-p);
}
}
