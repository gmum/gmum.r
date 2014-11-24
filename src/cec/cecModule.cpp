#ifndef CEC_MODULE_H
#define CEC_MODULE_H

#include <RcppCommon.h>
using namespace Rcpp;

class cecConfiguration;
class cecClient;
class cecModel;

RCPP_EXPOSED_CLASS(cecConfiguration)
RCPP_EXPOSED_CLASS(cecClient)
RCPP_EXPOSED_CLASS(cecModel)

#include "cecClient.hpp"
#include "cecConfiguration.hpp"
#include "cec.hpp"
using namespace gmum;

RCPP_MODULE(cec) {
    class_<cecConfiguration>("cecConfiguration")
	.constructor()
    .method("setDataSet", &cecConfiguration::setDataSet)
    .method("setNrOfClusters", &cecConfiguration::setNrOfClusters)
    .method("setMethodInit", &cecConfiguration::setMethodInit)
    .method("setR", &cecConfiguration::setR)
    .method("setCov", &cecConfiguration::setCov)
    .method("setCentroids", &cecConfiguration::setCentroids)
    .method("setMethodType", &cecConfiguration::setMethodType)
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
    .constructor<cecConfiguration*>()
    .method("findBestCEC", &cecClient::findBestCEC);

    std::list<double> (cecModel::*predict_1)(std::vector<double>,
            bool) = &cecModel::predict;
    unsigned int (cecModel::*predict_2)(
            std::vector<double>) const = &cecModel::predict;

    //TODO
    class_<cecModel>("cecModel")
    .method("loop", &cecModel::loop)
    .method("singleLoop", &cecModel::singleLoop)
    .method("entropy", &cecModel::entropy)
    .method("y", &cecModel::getAssignment)
    .method("clustering",&cecModel::getAssignment)
    .method("centers", &cecModel::centers)
    .method("cov", &cecModel::cov)
	.method("predict", predict_1)
	.method("predict", predict_2)
    .method("log.ncluster", &cecModel::getNrOfClusters)
    .method("log.energy", &cecModel::getEnergy)
    .method("log.iters", &cecModel::iters);
    //.method("x", getDataSet)
}
#endif
