#include <Rcpp.h>
#include <armadillo>
#include "Hartigan.hpp"
#include "CEC.hpp"
#include "random_assignment.hpp"
#include "kmeanspp_assignment.hpp"
#include <list>
#include <vector>
#include <boost/smart_ptr.hpp>

namespace gmum {

  struct CONST {
    static const char* dataset;
    static const char* nrOfClusters;
    static const char* clusters;
    static const char* nstart;
    static const char* killThreshold;
    static const char* itmax;
    static const char* energy;
    static const char* nclusters;

    struct CLUSTERS {
      static const char* type;
      static const char* standard;
      static const char* full;
      static const char* diagonal;
      static const char* spherical;
      static const char* fsphere;

      static const char* init;
      static const char* kmeanspp;
      static const char* random;

      static const char* covMat;
      static const char* radius;

    };
  };

  enum Assignment{
    kmeanspp, random
  };


  CEC* CEC__new(SEXP args);

  void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list);

  void initVectors(std::vector<ClusterType> &type, 
		   std::vector<arma::mat> &covMat,
		   std::vector<float> &radius,
		   std::list<Rcpp::List> clusters);

  void randomAssignment(Assignment assignmentType, std::vector<unsigned int> &assignment,
			arma::mat &points, int nrOfClusters);

  std::list<float> CECpredict(CEC*, std::vector<float>, bool);
  unsigned int CECpredict(CEC*, std::vector<float>);


  const char* CONST::dataset = "x";
  const char* CONST::nrOfClusters = "k";
  const char* CONST::clusters = "params.mix";
  const char* CONST::nstart = "params.nstart";
  const char* CONST::killThreshold = "control.eps";
  const char* CONST::itmax = "control.itmax";
  const char* CONST::energy = "log.energy";
  const char* CONST::nclusters = "log.ncluster";

  const char* CONST::CLUSTERS::type = "method.type";
  const char* CONST::CLUSTERS::standard = "std";
  const char* CONST::CLUSTERS::full = "full";
  const char* CONST::CLUSTERS::diagonal = "diagonal";
  const char* CONST::CLUSTERS::spherical = "spherical";
  const char* CONST::CLUSTERS::fsphere = "fsphere";

  const char* CONST::CLUSTERS::init = "method.init";
  const char* CONST::CLUSTERS::kmeanspp = "kmeans++";
  const char* CONST::CLUSTERS::random = "random";

  const char* CONST::CLUSTERS::covMat = "params.cov";
  const char* CONST::CLUSTERS::radius = "params.r";
  

  RCPP_MODULE(cec) {
    using namespace Rcpp;

    std::list<float> (*predict_1)(CEC*, std::vector<float>, bool) = CECpredict;
    unsigned int (*predict_2)(CEC*, std::vector<float>) = CECpredict;

    class_<CEC>("cec")
      .factory(CEC__new)
      .method("loop", &CEC::loop)
      .method("singleLoop", &CEC::singleLoop)
      .method("entropy", &CEC::entropy)
      .method("y", &CEC::getAssignment)
      .method("centers", &CEC::centers)
      .method("cov", &CEC::cov)
      .method("predict", predict_1)
      .method("predict", predict_2)
      .method("log.ncluster", &CEC::getNrOfClusters)
      .method("log.energy", &CEC::getEnergy)
      .method("log.iters", &CEC::iters)
      .method("clustering",&CEC::getAssignment)
      ;
  }
}
