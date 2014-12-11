#ifndef CEC_HPP
#define CEC_HPP

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

/**
 * Instance of this class is CEC model object.
 */
class cecModel {
private:
    gmum::TotalResult result;
	boost::shared_ptr<std::vector<unsigned int> > assignment;
	boost::shared_ptr<const arma::mat> points;
    boost::shared_ptr<gmum::Algorithm> algorithm;
    double killThreshold;
	std::vector<bool> invSet;
	std::vector<arma::mat> inv;
    boost::shared_ptr<gmum::Cluster> createCluster(const gmum::ClusterParams &params,
			int i);
    cecConfiguration config;

    void findBestCEC();
    void init(boost::shared_ptr<gmum::Algorithm> algorithm,
              boost::shared_ptr<std::vector<unsigned int> > assignment);
public:
    cecModel(cecConfiguration* cfg);
    cecModel(const cecModel& other);
    cecModel& operator=(const cecModel& other);
    std::vector<boost::shared_ptr<gmum::Cluster> > clusters;
    void loop();
	void singleLoop();
	double entropy();
	std::vector<unsigned int> &getAssignment() const;
	void setAssignment(std::vector<unsigned int> assignment);
    arma::mat getPoints();
	std::vector<arma::rowvec> centers() const;
	std::vector<arma::mat> cov() const;
	unsigned int iters() const;
	std::list<unsigned int> getNrOfClusters() const;
    std::list<double> getEnergy() const;
	unsigned int predict(std::vector<double> vec) const;
    std::list<double> predict(std::vector<double> vec, bool general);
};

#endif
