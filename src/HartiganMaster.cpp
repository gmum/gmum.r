#include <RcppArmadillo.h>
#include <random>
#include "Hartigan.hpp"

RcppExport SEXP run(SEXP &numberOfClusters, SEXP &numberOfTimes, SEXP &dataset) {

  int nrOfClusters = Rcpp::as<int>(numberOfClusters);
  Rcpp::NumericMatrix points = Rcpp::as<Rcpp::NumericMatrix>(dataset);

  //random assignment into clusters
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, nrOfClusters);

  std::vector<int> fits;
  fits.reserve(points.nrow());

  for(int i = 0; i < points.nrow(); i++) fits[i] = distribution(generator);

  //Hartigan algorithm
  Hartigan algorithm(nrOfClusters, 0.0001, fits, points);

  algorithm.loop();

  return points;
}
