#ifndef CEC_HPP
#define CEC_HPP

#include <list>
#include <vector>
#include <RcppArmadillo.h>
#include "boost/foreach.hpp"
#include "boost/smart_ptr.hpp"
#include "algorithm.hpp"
#include "cluster.hpp"
#include "clusterCustomFunction.hpp"
#include "clusterParams.hpp"
#include "exceptions.hpp"
#include "params.hpp"

namespace gmum {

/**
 * Instance of this class is CEC model object.
 */
class cecModel {
private:
	TotalResult result;
	boost::shared_ptr<std::vector<unsigned int> > assignment;
	boost::shared_ptr<const arma::mat> points;
	boost::shared_ptr<Algorithm> algorithm;
	const double killThreshold;
	std::vector<bool> invSet;
	std::vector<arma::mat> inv;
	boost::shared_ptr<Cluster> createCluster(const ClusterParams &params,
			int i);
public:
	cecModel(boost::shared_ptr<Algorithm> algorithm,
			boost::shared_ptr<std::vector<unsigned int> > assignment,
			const Params &params);

	std::vector<boost::shared_ptr<Cluster> > clusters;
	void loop();
	void singleLoop();
	double entropy();
	std::vector<unsigned int> &getAssignment() const;
	const arma::mat &getPoints() const;
	std::vector<arma::rowvec> centers() const;
	std::vector<arma::mat> cov() const;
	unsigned int iters() const;
	std::list<unsigned int> getNrOfClusters() const;
	boost::shared_ptr<const arma::mat> getPtrToPoints() const;
	boost::shared_ptr<std::vector<unsigned int> > getPtrToAssignement() const;
	std::list<double> getEnergy() const;
	unsigned int predict(std::vector<double> vec) const;
	std::list<double> predict(std::vector<double> vec, bool general);
};

}

#endif
