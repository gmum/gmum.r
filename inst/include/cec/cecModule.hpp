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

  /**
   * connection between c++ and R. Is responsible for creating CEC module object.
   * First proceses arguments, second verify whether arguments are correct.
   * Third find best clustering
   */
  CEC* CEC__new(SEXP args);

  /**
   * number of arguments is quite large. Because of that processing has been
   * exported to seperate function.
   */
  Params processArguments(const Rcpp::List &list);
  /**
   * depending on arguments some data maybe necessary. Verify checks whether
   * arguments given are sufficient to learn the model.
   */
  void verifyParams(const Params &params);
  /**
   * assign points to clusters and learn model nstart number of times.
   */
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
