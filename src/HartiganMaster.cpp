#include <RcppArmadillo.h>
#include <time.h>
#include <stdlib.h>
#include "Hartigan.hpp"

RcppExport SEXP run(SEXP &numberOfClusters, SEXP &numberOfTimes, SEXP &dataset) {

  int nrOfClusters = Rcpp::as<int>(numberOfClusters);
  Rcpp::NumericMatrix points = Rcpp::as<Rcpp::NumericMatrix>(dataset);

  //random assignment into clusters
  int seed = time(NULL);
  srand(seed);

  std::vector<int> fits;
  fits.reserve(points.nrow());

  for(int i = 0; i < points.nrow(); i++) fits[i] = rand()%nrOfClusters;

  //Hartigan algorithm
  Hartigan algorithm(nrOfClusters, 0.0001, fits, points);

  algorithm.loop();

  return points;
}
