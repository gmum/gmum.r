#ifndef CEC_MODULE_H
#define CEC_MODULE_H

#include <RcppCommon.h>
using namespace Rcpp;

class cecConfiguration;
class cecModel;

RCPP_EXPOSED_CLASS (cecConfiguration)
RCPP_EXPOSED_CLASS (cecModel)

#include "cecConfiguration.hpp"
#include "cec.hpp"
using namespace gmum;

RCPP_MODULE(cec) {
	class_<cecConfiguration>("cecConfiguration")
	.constructor()
	.method("setDataSet", &cecConfiguration::setDataSet)
	.method("setEps", &cecConfiguration::setEps)
	.method("setMix", &cecConfiguration::setMix)
	.method("setNrOfClusters", &cecConfiguration::setNrOfClusters)
	.method("setLogEnergy", &cecConfiguration::setLogEnergy)
	.method("setLogCluster", &cecConfiguration::setLogCluster)
	.method("setNstart", &cecConfiguration::setNstart)
	.method("setCentroids", &cecConfiguration::setCentroids)
	.method("setMethodInit", &cecConfiguration::setMethodInit)
	.method("setMethodType", &cecConfiguration::setMethodType)
	.method("setCov", &cecConfiguration::setCov)
	.method("setR", &cecConfiguration::setR)
	.method("setFunction", &cecConfiguration::setFunction)
	.method("setItmax", &cecConfiguration::setItmax)
	.method("setIters", &cecConfiguration::setIters);
	//.field("x", &cecConfiguration::data)

	std::list<double> (cecModel::*predict_1)(std::vector<double>,
			bool) = &cecModel::predict;
	unsigned int (cecModel::*predict_2)(
			std::vector<double>) const = &cecModel::predict;

	class_<cecModel>("cecModel")
	.constructor<cecConfiguration*>()
	.method("loop", &cecModel::loop)
	.method("singleLoop", &cecModel::singleLoop)
	.method("entropy", &cecModel::entropy)
    .method("energy", &cecModel::getEnergy)
	.method("y", &cecModel::getAssignment)
	.method("clustering",&cecModel::getAssignment)
	.method("centers", &cecModel::centers)
	.method("cov", &cecModel::cov)
	.method("predict", predict_1)
	.method("predict", predict_2)
	.method("log.ncluster", &cecModel::getNrOfClusters)
	.method("log.energy", &cecModel::getEnergy)
	.method("log.iters", &cecModel::iters)
    .method("x", &cecModel::getPoints);

}
#endif
