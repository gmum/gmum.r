#include <Rcpp.h>
#include <armadillo>
#include <random>
#include "Hartigan.hpp"

RcppExport SEXP run(SEXP &numberOfClusters, SEXP &numberOfTimes, SEXP &dataset) {

  int nrOfClusters = Rcpp::as<int>(numberOfClusters);
  Rcpp::NumericMatrix proxyDataset(dataset);
  //reuses memory and avoids extra copy
  arma::mat points(proxyDataset.begin(), proxyDataset.nrow(), proxyDataset.ncol(), false);

  //random assignment into clusters
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, nrOfClusters);

  std::vector<int> fits;
  fits.reserve(points.n_rows);

  for(int i = 0; i < points.n_rows; i++) fits[i] = distribution(generator);

  //Hartigan algorithm
  Hartigan algorithm(nrOfClusters, 0.0001, fits, points);

  algorithm.loop();

  return Rcpp::List::create(fits);
}
