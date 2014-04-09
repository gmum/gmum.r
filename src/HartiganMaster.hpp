#include <Rcpp.h>
#include <armadillo>
#include "Hartigan.hpp"
#include "CEC.hpp"
#include <list>
#include <vector>

RcppExport SEXP run(SEXP args);

void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list);

void initVectors(std::vector<ClusterType> &type, 
		 std::vector<arma::mat> &covMat,
		 std::vector<float> &radius,
		 std::list<Rcpp::List> clusters);


