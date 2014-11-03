#ifndef CEC_MODULE_H
#define CEC_MODULE_H

#include "cecClient.hpp"

RCPP_EXPOSED_CLASS (cecConfiguration);
RCPP_EXPOSED_CLASS (cecClient);
RCPP_EXPOSED_CLASS (CEC);

namespace gmum {

RCPP_MODULE(cec) {
	using namespace Rcpp;

	class_<cecConfiguration>("cecConfiguration")
	.constructor()
	.method("setX", &cecConfiguration::setX)
	.method("setK", &cecConfiguration::setK)
	.method("setMethodType", &cecConfiguration::setMethodType)
	.method("setMethodInit", &cecConfiguration::setMethodInit)
	.method("setR", &cecConfiguration::setR)
	.method("setCov", &cecConfiguration::setCov)
	.method("setCentroids", &cecConfiguration::setCentroids)
	.method("setMix", &cecConfiguration::setMix)
	.method("setFunction", &cecConfiguration::setFunction)
	.method("setNstart", &cecConfiguration::setNstart)
	.method("setEps", &cecConfiguration::setEps)
	.method("setItmax", &cecConfiguration::setItmax)
	.method("setLogEnergy", &cecConfiguration::setLogEnergy)
	.method("setNCluster", &cecConfiguration::setNCluster)
	.method("setIters", &cecConfiguration::setIters);
	//.field("x", &cecConfiguration::data)

	class_<cecClient>("cecClient")
	.constructor()
	.method("findBestCEC", &cecClient::findBestCEC);

	std::list<double> (CEC::*predict_1)(std::vector<double>,
			bool) = &CEC::predict;
	unsigned int (CEC::*predict_2)(std::vector<double>) const = &CEC::predict;
	//TODO
	class_<CEC>("CEC")
	.method("loop", &CEC::loop)
	.method("singleLoop", &CEC::singleLoop)
	.method("entropy", &CEC::entropy)
	.method("y", &CEC::getAssignment)
	.method("clustering",&CEC::getAssignment)
	.method("centers", &CEC::centers)
	.method("cov", &CEC::cov)
	.method("predict", predict_1)
	.method("predict", predict_2)
	.method("log.ncluster", &CEC::getNrOfClusters)
	.method("log.energy", &CEC::getEnergy)
	.method("log.iters", &CEC::iters);
	//.method("x", getDataSet)

}
}
#endif
