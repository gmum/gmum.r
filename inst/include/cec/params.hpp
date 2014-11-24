#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <string>
#include <list>
#include <vector>
#include "boost/smart_ptr.hpp"
#include "clusterParams.hpp"

namespace gmum {

enum AssignmentType {
	kkmeanspp, krandom, kcentroids, knoAssignment,
};

struct Params {
public:
	boost::shared_ptr<const arma::mat> dataset;
	double killThreshold;
	unsigned int nrOfClusters;
	bool logNrOfClusters;
	bool logEnergy;
	unsigned int nstart;
	AssignmentType assignmentType;
	bool centroidsSet;
	std::list<std::vector<double> > centroids;
	ClusterType clusterType;
	std::list<boost::shared_ptr<ClusterParams> > clusters;
	bool covMatSet;
	arma::mat covMat;
	bool radiusSet;
	double radius;
	bool functionNameSet;
	std::string functionName;
};

}

#endif
