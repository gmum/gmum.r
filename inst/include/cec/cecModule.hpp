#include <list>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <RcppArmadillo.h>
#include "cec.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "kmeansppAssignment.hpp"
#include "centroidsAssignment.hpp"
#include "clusterParams.hpp"
#include "params.hpp"

namespace gmum {

  struct CONST {
    static const char* dataset;
    static const char* nrOfClusters;
    static const char* clusters;
    static const char* nstart;
    static const char* centroidsList;
    static const char* killThreshold;
    static const char* itmax;
    static const char* energy;
    static const char* logClusters;

    struct CLUSTERS {
      static const char* type;
      static const char* standard;
      static const char* full;
      static const char* diagonal;
      static const char* sphere;
      static const char* fsphere;

      static const char* init;
      static const char* kmeanspp;
      static const char* random;
      static const char* centroids;

      static const char* covMat;
      static const char* radius;
    };

    static const unsigned int nrOfClustersInit;
    static const double killThresholdInit;
    static const unsigned int nstartInit;
    static const AssignmentType defaultAssignment;

    struct ERRORS {
      static const char* datasetReq;
      static const char* nstartsPositive;
      static const char* nrOfClustersPositive;
      static const char* datasetSize;
      static const char* killThresholdSize;
      static const char* assignmentError;
      static const char* covMatReq;
      static const char* radiusReq;
      static const char* clusterRecError;
      static const char* centroidsError;
    };
  };



  CEC* CEC__new(SEXP args);

  Params processArguments(const Rcpp::List &list);
  void verifyParams(const Params &params);
  CEC *findBestCEC(const Params &params);

  arma::mat getDataSet(CEC* cec);

  std::list<double> CECpredict(CEC*, std::vector<double>, bool);
  unsigned int CECpredict(CEC*, std::vector<double>);

  unsigned int getNstart(CEC* cec);

  const char* CONST::dataset = "x";
  const char* CONST::nrOfClusters = "k";
  const char* CONST::clusters = "params.mix";
  const char* CONST::nstart = "params.nstart";  
  const char* CONST::centroidsList = "params.centroids";
  const char* CONST::killThreshold = "control.eps";
  const char* CONST::itmax = "control.itmax";
  const char* CONST::energy = "log.energy";
  const char* CONST::logClusters = "log.ncluster";

  const char* CONST::CLUSTERS::type = "method.type";
  const char* CONST::CLUSTERS::standard = "std";
  const char* CONST::CLUSTERS::full = "full";
  const char* CONST::CLUSTERS::diagonal = "diagonal";
  const char* CONST::CLUSTERS::sphere = "sphere";
  const char* CONST::CLUSTERS::fsphere = "fsphere";

  const char* CONST::CLUSTERS::init = "method.init";
  const char* CONST::CLUSTERS::kmeanspp = "kmeans++";
  const char* CONST::CLUSTERS::random = "random";
  const char* CONST::CLUSTERS::centroids = "centroids";

  const char* CONST::CLUSTERS::covMat = "params.cov";
  const char* CONST::CLUSTERS::radius = "params.r";
  
  const unsigned int CONST::nrOfClustersInit = 10;
  const double CONST::killThresholdInit = 1e-4;
  const unsigned int CONST::nstartInit = 1;
  const AssignmentType CONST::defaultAssignment = kmeanspp;

  const char* CONST::ERRORS::datasetReq = "dataset is required!";
  const char* CONST::ERRORS::nstartsPositive = "Number of starts should be a positive integer!";
  const char* CONST::ERRORS::nrOfClustersPositive = "Number of clusters should be a positive integer!";
  const char* CONST::ERRORS::datasetSize = "Size of dataset cannot be less than number of clusters!";
  const char* CONST::ERRORS::killThresholdSize = (std::string(CONST::killThreshold)+" is too hight").c_str();
  const char* CONST::ERRORS::assignmentError = "cannot recognize assignment initiation method";
  const char* CONST::ERRORS::covMatReq = "cavariance matrix required";
  const char* CONST::ERRORS::radiusReq = "radius is required";
  const char* CONST::ERRORS::clusterRecError = "cannot recognise cluster type";
  const char* CONST::ERRORS::centroidsError = "number of centroids is different than number of clusters";

  unsigned int nstart;

  RCPP_MODULE(cec) {
    using namespace Rcpp;

    std::list<double> (*predict_1)(CEC*, std::vector<double>, bool) = CECpredict;
    unsigned int (*predict_2)(CEC*, std::vector<double>) = CECpredict;

    class_<CEC>("cec")
      .factory(CEC__new)
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
      .method("log.iters", &CEC::iters)
      .method("x", getDataSet)
      .method("nstart", getNstart)
      ;
  }
}
