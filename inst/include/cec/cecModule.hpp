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
#include "const.hpp"

namespace gmum {

  CEC* CEC__new(SEXP args);

  Params processArguments(const Rcpp::List &list);
  void verifyParams(const Params &params);
  CEC *findBestCEC(const Params &params);

  arma::mat getDataSet(CEC* cec);

  std::list<double> CECpredict(CEC*, std::vector<double>, bool);
  unsigned int CECpredict(CEC*, std::vector<double>);

  unsigned int getNstart(CEC* cec);
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
