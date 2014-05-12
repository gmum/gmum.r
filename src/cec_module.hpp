#include <Rcpp.h>
#include <armadillo>
#include "Hartigan.hpp"
#include "CEC.hpp"
#include <list>
#include <vector>

CEC* CEC__new(SEXP args);

void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list);

void initVectors(std::vector<ClusterType> &type, 
		 std::vector<arma::mat> &covMat,
		 std::vector<float> &radius,
		 std::list<Rcpp::List> clusters);

struct CONST {
  static const char* dataset;
  static const char* nrOfClusters;
  static const char* clusters;
  struct CLUSTERS {
    static const char* type;
    static const char* covMat;
    static const char* radius;
    static const char* usual;
    static const char* spherical;
    static const char* diagonal;
    static const char* constRadius;
  };
  static const char* killThreshold;
};

const char* CONST::dataset = "dataset";
const char* CONST::nrOfClusters = "K";
const char* CONST::clusters = "clusters";
const char* CONST::killThreshold = "killThreshold";

const char* CONST::CLUSTERS::type = "type";
const char* CONST::CLUSTERS::covMat = "covMat";
const char* CONST::CLUSTERS::radius = "radius";
const char* CONST::CLUSTERS::usual = "usual";
const char* CONST::CLUSTERS::spherical = "spherical";
const char* CONST::CLUSTERS::diagonal = "diagonal";
const char* CONST::CLUSTERS::constRadius = "constRadius";
  
RCPP_MODULE(cec) {
  using namespace Rcpp;
  class_<CEC>("cec")
    .factory(CEC__new)
    .method("test", &CEC::test)
    .method("loop", &CEC::loop)
    .method("singleLoop", &CEC::singleLoop)
    .method("entropy", &CEC::entropy)
    ;
}
