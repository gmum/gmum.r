#include <list>
#include <vector>
#include "boost/smart_ptr.hpp"
#include <RcppArmadillo.h>
#include "cec.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "kmeansppAssignment.hpp"
#include "centroidsAssignment.hpp"
#include "clusterParams.hpp"
#include "params.hpp"
#include "const.hpp"

namespace gmum {

  CEC* CEC__new(SEXP args);

  Params processArguments(const Rcpp::List &list);
  void verifyParams(const Params &params);
  CEC *findBestCEC(const Params &params);

  arma::mat getDataSet(CEC* cec);

  unsigned int getNstart(CEC* cec);
  unsigned int nstart;

  RCPP_MODULE(cec) {
    using namespace Rcpp;

    std::list<double> (CEC::*predict_1)(std::vector<double>, bool) = &CEC::predict;
    unsigned int (CEC::*predict_2)(std::vector<double>) const = &CEC::predict;

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
