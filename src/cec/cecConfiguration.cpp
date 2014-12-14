#include "cecConfiguration.hpp"
#include "const.hpp"
#include <boost/foreach.hpp>
#include <sstream>
#include <string.h>
#include <iostream>

using namespace gmum;

cecConfiguration::cecConfiguration() {
}

Params cecConfiguration::getParams() {
	return this->params;
}

void cecConfiguration::setParams(Params params) {
	this->params = params;
}

void cecConfiguration::setDataSet(const Rcpp::NumericMatrix proxyDataset) {
	//reuses memory and avoids extra copy
	boost::shared_ptr<const arma::mat> points(
			new arma::mat(proxyDataset.begin(), proxyDataset.nrow(),
					proxyDataset.ncol(), false));
	params.dataset = points;
}

void cecConfiguration::setEps(const double killThreshold) {
	params.killThreshold = killThreshold;
}

void cecConfiguration::setMix(const Rcpp::List clusters) {
	if (!Rf_isNull(clusters)) {
		Rcpp::List desc = Rcpp::as < Rcpp::List > (clusters);
		for (Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it) {
			Rcpp::List list = Rcpp::as < Rcpp::List > (*it);
			boost::shared_ptr<ClusterParams> cluster;
			if (!list.containsElementNamed(CONST::CLUSTERS::type)) {
				cluster.reset(new ClusterParams());
				cluster->type = knoType;
			} else {
				std::string typeStr = Rcpp::as < std::string
						> (list[CONST::CLUSTERS::type]);
				if (typeStr.compare(CONST::CLUSTERS::standard) == 0) {
					cluster.reset(new ClusterParams());
					cluster->type = kstandard;
				} else if (typeStr.compare(CONST::CLUSTERS::full) == 0) {
					ClusterFullParams *ptr = new ClusterFullParams();
					ptr->type = kfull;
					if (list.containsElementNamed(CONST::CLUSTERS::covMat)) {
						ptr->covMatSet = true;
						Rcpp::NumericMatrix temp = Rcpp::as
								< Rcpp::NumericMatrix
								> (list[CONST::CLUSTERS::covMat]);
						ptr->covMat = arma::mat(temp.begin(), temp.nrow(),
								temp.ncol());
					} else
						ptr->covMatSet = false;
					cluster.reset(ptr);
				} else if (typeStr.compare(CONST::CLUSTERS::fsphere) == 0) {
					ClusterFsphereParams *ptr = new ClusterFsphereParams();
					ptr->type = kfsphere;
					if (list.containsElementNamed(CONST::CLUSTERS::radius)) {
						ptr->radiusSet = true;
						ptr->radius = Rcpp::as<double>(
								list[CONST::CLUSTERS::radius]);
					} else
						ptr->radiusSet = false;
					cluster.reset(ptr);
				} else if (typeStr.compare(CONST::CLUSTERS::sphere) == 0) {
					cluster.reset(new ClusterParams());
					cluster->type = ksphere;
				} else if (typeStr.compare(CONST::CLUSTERS::diagonal) == 0) {
					cluster.reset(new ClusterParams());
					cluster->type = kdiagonal;
				} else if (typeStr.compare(CONST::CLUSTERS::custom) == 0) {
					ClusterCustomParams *ptr = new ClusterCustomParams();
					ptr->type = kcustom;
					if (list.containsElementNamed(CONST::CLUSTERS::functionName)
							== 0) {
						ptr->functionNameSet = true;
						ptr->functionName = Rcpp::as < std::string
								> (list[CONST::CLUSTERS::functionName]);
					} else {
						ptr->functionNameSet = false;
					}
					cluster.reset(ptr);
				} else {
					cluster.reset(new ClusterParams());
					cluster->type = knoType;
				}
			}

			params.clusters.push_back(cluster);
		}
	}
}

void cecConfiguration::setNrOfClusters(const unsigned int nrOfClusters) {
	if (nrOfClusters != 0)
		params.nrOfClusters = nrOfClusters;
	else if (params.clusters.size() > 0)
		params.nrOfClusters = params.clusters.size();
	else
		params.nrOfClusters = CONST::nrOfClustersInit;

	if (params.dataset->n_rows < params.nrOfClusters)
		Rcpp::stop(CONST::ERRORS::datasetSize);
}

void cecConfiguration::setLogEnergy(bool logEnergy) {
	params.logEnergy = logEnergy;
}

void cecConfiguration::setLogCluster(bool logNrOfClusters) {
	params.logNrOfClusters = logNrOfClusters;
}

void cecConfiguration::setNstart(const unsigned int nstart) {
	params.nstart = nstart;
}

void cecConfiguration::setCentroids(const Rcpp::List centroids) {
	if (!Rf_isNull(centroids)) {
		Rcpp::List desc = Rcpp::as < Rcpp::List > (centroids);
		for (Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
			params.centroids.push_back(Rcpp::as < std::vector<double> > (*it));
		params.centroidsSet = true;
	} else
		params.centroidsSet = false;
}

void cecConfiguration::setMethodInit(const std::string init) {
	params.assignmentType = CONST::defaultAssignment;
	if (init.compare(CONST::CLUSTERS::random) == 0)
		params.assignmentType = krandom;
	else if (init.compare(CONST::CLUSTERS::kmeanspp) == 0)
		params.assignmentType = kkmeanspp;
	else if (init.compare(CONST::CLUSTERS::centroids) == 0) {
		params.assignmentType = kcentroids;
	}

	if (params.assignmentType == knoAssignment)
		Rcpp::stop(CONST::ERRORS::assignmentError);
	if (params.assignmentType == kcentroids
			&& params.centroids.size() != params.nrOfClusters)
		Rcpp::stop(CONST::ERRORS::centroidsError);
	switch (params.clusterType) {
	case kstandard: // TODO: handle kstandard parameter
	case kdiagonal: // TODO: handle kdiagonal parameter
	case ksphere: // TODO: handle ksphere parameter
		break;
	case kfull:
		if (!params.covMatSet)
			Rcpp::stop(CONST::ERRORS::covMatReq);
		break;
	case kfsphere:
		if (!params.radiusSet)
			Rcpp::stop(CONST::ERRORS::radiusReq);
		break;
	case knoType:
		Rcpp::stop(CONST::ERRORS::clusterRecError);
		break;
	case kcustom:
		if (!params.functionNameSet)
			Rcpp::stop(CONST::ERRORS::functionNameReq);
		break;
	case kmix:
		BOOST_FOREACH(boost::shared_ptr < ClusterParams > cluster,
				params.clusters)
		{
			switch (cluster->type) {
			case kstandard:
			case kdiagonal:
			case ksphere:
			case kmix:
				break;
			case kfull: {
				ClusterFullParams &ptr =
						static_cast<ClusterFullParams&>(*cluster);
				if (!ptr.covMatSet)
					Rcpp::stop(CONST::ERRORS::covMatReq);
				break;
			}
			case kfsphere: {
				ClusterFsphereParams &ptr =
						static_cast<ClusterFsphereParams&>(*cluster);
				if (!ptr.radiusSet)
					Rcpp::stop(CONST::ERRORS::radiusReq);
				break;
			}
			case knoType:
				Rcpp::stop(CONST::ERRORS::clusterRecError);
				break;
			case kcustom:
				ClusterCustomParams &ptr =
						static_cast<ClusterCustomParams&>(*cluster);
				if (!ptr.functionNameSet)
					Rcpp::stop(CONST::ERRORS::functionNameReq);
				break;
			}
		}
	}
}

void cecConfiguration::setMethodType(const std::string type) {
	if (params.clusters.size() > 0)
		params.clusterType = kmix;
	else {
		params.clusterType = kstandard;
		if (type.empty())
			return;
		else if (type.compare(CONST::CLUSTERS::standard) == 0) {
			//nothing but recognises
		} else if (type.compare(CONST::CLUSTERS::full) == 0) {
			params.clusterType = kfull;
		} else if (type.compare(CONST::CLUSTERS::diagonal) == 0) {
			params.clusterType = kdiagonal;
		} else if (type.compare(CONST::CLUSTERS::sphere) == 0) {
			params.clusterType = ksphere;
		} else if (type.compare(CONST::CLUSTERS::fsphere) == 0) {
			params.clusterType = kfsphere;
		} else if (type.compare(CONST::CLUSTERS::custom) == 0) {
			params.clusterType = kcustom;
		} else{
			Rcpp::stop(CONST::ERRORS::clusterRecError);
		}
	}
}
void cecConfiguration::setCov(const Rcpp::NumericMatrix covMatProxy) {
//TODO better check is empty matrix
	if (!Rf_isNull(covMatProxy) && params.clusters.empty()) {
		params.covMatSet = true;
		params.covMat = arma::mat(covMatProxy.begin(), covMatProxy.nrow(),
				covMatProxy.ncol());
	} else
		params.covMatSet = false;
}

void cecConfiguration::setR(const double radius) {
	if (radius != 0 && params.clusters.empty()) {
		params.radiusSet = true;
		params.radius = radius;
	} else
		params.radiusSet = false;
}

void cecConfiguration::setFunction(const std::string functionName) {
	if (!functionName.empty() && params.clusters.empty()) {
		params.functionNameSet = true;
		params.functionName = functionName;
	} else {
		params.functionNameSet = false;
	}
}

void cecConfiguration::setItmax(const unsigned int) {
}

void cecConfiguration::setIters(bool iters) {
}

