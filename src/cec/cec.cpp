#include "cec.hpp"
#include "hartigan.hpp"
#include "assignment.hpp"
#include "kmeansppAssignment.hpp"
#include "centroidsAssignment.hpp"
#include "randomAssignment.hpp"

using namespace gmum;

boost::shared_ptr<Cluster> cecModel::createCluster(const ClusterParams &params,
		int i) {
	boost::shared_ptr<Cluster> cluster;
	switch (params.type) {
	case knoType: // TODO: handle knoType parameter
	case kmix: // TODO: handle kmix parameter
		break;
	case kstandard:
		cluster = boost::shared_ptr < Cluster
				> (new ClusterStandard(i, *assignment, *points));
		break;
	case kfull: {
		const ClusterFullParams &ptr =
				static_cast<const ClusterFullParams&>(params);
		cluster = boost::shared_ptr < Cluster
				> (new ClusterCovMat(ptr.covMat, i, *assignment, *points));
		break;
	}
	case kdiagonal:
		cluster = boost::shared_ptr < Cluster
				> (new ClusterDiagonal(i, *assignment, *points));
		break;
	case ksphere:
		cluster = boost::shared_ptr < Cluster
				> (new ClusterSpherical(i, *assignment, *points));
		break;
	case kfsphere: {
		const ClusterFsphereParams &ptr =
				static_cast<const ClusterFsphereParams&>(params);
		cluster = boost::shared_ptr < Cluster
				> (new ClusterConstRadius(ptr.radius, i, *assignment, *points));
		break;
	}
	case kcustom: {
		const ClusterCustomParams &ptr =
				static_cast<const ClusterCustomParams&>(params);
		cluster = boost::shared_ptr < Cluster
				> (new ClusterCustomFunction(i, *assignment, *points,
						ptr.functionName));
		break;
	}
	}
	return cluster;
}
cecModel::cecModel(cecConfiguration *cfg) :
		config(*cfg) {
	findBestCEC();
}
cecModel::cecModel(const cecModel &other) {
	*this = other;
}

cecModel& cecModel::operator=(const cecModel& other) {
	result = other.result;
	assignment = boost::shared_ptr < std::vector<unsigned int>
			> (new std::vector<unsigned int>(*other.assignment));
	points = other.points;
	algorithm = other.algorithm;
	killThreshold = other.killThreshold;
	invSet = other.invSet;
	inv = other.inv;
	config = other.config;
	clusters = other.clusters;
	return *this;
}

void cecModel::init(boost::shared_ptr<Algorithm> algorithm,
		boost::shared_ptr<std::vector<unsigned int> > assignment) {
	Params params = config.getParams();
	this->assignment = assignment;
	this->points = params.dataset;
	this->algorithm = algorithm;
	this->killThreshold = params.killThreshold;
	clusters.clear();
	clusters.reserve(params.nrOfClusters);

	int i = 0;
	if (params.clusterType == kmix) {
		BOOST_FOREACH(boost::shared_ptr < ClusterParams > cluster,
				params.clusters)
		{
			clusters.push_back(createCluster(*cluster, i));
		}
	} else {
		ClusterParams *cluster;
		switch (params.clusterType) {
		case kfsphere: {
			ClusterFsphereParams *proxy = new ClusterFsphereParams();
			proxy->radius = params.radius;
			cluster = proxy;
			break;
		}
		case kfull: {
			ClusterFullParams *proxy = new ClusterFullParams();
			proxy->covMat = params.covMat;
			cluster = proxy;

			break;
		}
		case kcustom: {
			ClusterCustomParams *proxy = new ClusterCustomParams();
			proxy->functionName = params.functionName;
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
		cluster->type = params.clusterType;
		for (unsigned int i = 0; i < params.nrOfClusters; ++i)
			clusters.push_back(createCluster(*cluster, i));
		delete cluster;
	}
}

void cecModel::findBestCEC() {
	boost::shared_ptr < std::vector<unsigned int>
			> assignment(new std::vector<unsigned int>());
	boost::shared_ptr<Hartigan> hartigan(
			new Hartigan(this->config.getParams().logNrOfClusters,
					this->config.getParams().logEnergy));

	Assignment *assignmentType = NULL;
	switch (this->config.getParams().assignmentType) {
	case krandom:
		assignmentType = new RandomAssignment(*(config.getParams().dataset),
				config.getParams().nrOfClusters);
		break;
	case kkmeanspp:
		assignmentType = new KmeansppAssignment(*(config.getParams().dataset),
				config.getParams().nrOfClusters);
		break;
	case kcentroids:
		assignmentType = new CentroidsAssignment(*(config.getParams().dataset),
				config.getParams().centroids);
		break;
	case knoAssignment:
		break; // TODO: handle no assignment
	}

	assignment->resize(config.getParams().dataset->n_rows);
	(*assignmentType)(*assignment);

	init(hartigan, assignment);

	try {
		loop();
		cecModel best_cec = *this;
        gmum::Params params = config.getParams();
        for (unsigned int i = 1; i < params.nstart; ++i) {
			(*assignmentType)(*assignment);
			init(hartigan, assignment);
			loop();

            if (result.min_energy < best_cec.getResult().min_energy) {
				best_cec = *this;
			}
		}
		*this = best_cec;
	} catch (std::exception &e) {
		Rcpp::stop(std::string("exception ") + e.what() + " caught in CEC_new");
	}
	delete assignmentType;
}

void cecModel::loop() {
	result = algorithm->loop(*points, *assignment, killThreshold, clusters);
	int nrOfClusters = clusters.size();
	invSet.resize(nrOfClusters, false);
	inv.resize(nrOfClusters);
}

void cecModel::singleLoop() {
	algorithm->singleLoop(*points, *assignment, killThreshold, clusters);
}

double cecModel::entropy() {
	double s = 0.0;

	BOOST_FOREACH(boost::shared_ptr < Cluster > cluster, clusters)
	{
		s += cluster->entropy();
	}
	return s;
}

std::vector<unsigned int> &cecModel::getAssignment() const {
	return *assignment;
}

void cecModel::setAssignment(std::vector<unsigned int> assignment) {
	//TODO set assignment
}

arma::mat cecModel::getPoints() {
	return *points;
}

std::vector<arma::rowvec> cecModel::centers() const {
	std::vector < arma::rowvec > array;
	array.reserve(clusters.size());
	for (unsigned int i = 0; i < clusters.size(); ++i) {
		array.push_back(clusters[i]->getMean());
	}
	return array;
}

std::vector<arma::mat> cecModel::cov() const {
	std::vector < arma::mat > array;
	array.reserve(clusters.size());

	for (unsigned int i = 0; i < clusters.size(); ++i) {
		array.push_back(clusters[i]->getCovMat(i, *assignment, *points));
	}

	return array;
}

unsigned int cecModel::iters() const {
	return result.iterations;
}

std::list<unsigned int> cecModel::getNrOfClusters() const {
	return result.nrOfClusters;
}

std::list<double> cecModel::getEnergy() const {
	return result.energy;
}

unsigned int cecModel::predict(std::vector<double> vec) const {
	arma::rowvec x = arma::conv_to < arma::rowvec > ::from(vec);

	int assign = 0;
	double minEntropyChange = std::numeric_limits<double>::max();

	for (unsigned int i = 0; i < clusters.size(); ++i) {

		boost::shared_ptr<Cluster> oldCluster = clusters[i];
		boost::shared_ptr<Cluster> newCluster = clusters[i]->addPoint(x);
		double entropyChange = newCluster->entropy() - oldCluster->entropy();

		if (entropyChange < minEntropyChange) {
			minEntropyChange = entropyChange;
			assign = i;
		}
	}

	return assign;
}

std::list<double> cecModel::predict(std::vector<double> vec, bool general) {
	arma::rowvec x = arma::conv_to < arma::rowvec > ::from(vec);
	std::list<double> out;

	if (general)
		for (unsigned int i = 0; i < clusters.size(); ++i) {
			arma::mat covMat = clusters[i]->getCovMat(i, *assignment, *points);
			arma::rowvec mean = clusters[i]->getMean();
			if (!invSet[i]) {
				arma::mat Q, R;
				arma::qr_econ(Q, R, covMat);
				int dim = mean.n_cols;
				arma::mat Id = arma::eye(dim, dim);
				inv[i] = solve(R, Id) * Q.t();
				invSet[i] = true;
			}

			double constMultiplier = sqrt(
					1.0 / (pow(2 * M_PI, x.n_cols) * arma::det(covMat)));
			double scalar = arma::as_scalar(
					(x - mean) * inv[i] * ((x - mean).t()));
			double exponens = exp(-0.5 * scalar);

			out.push_back(constMultiplier * exponens);
		}

	return out;
}

const gmum::TotalResult& cecModel::getResult() const
{
    return result;
}
