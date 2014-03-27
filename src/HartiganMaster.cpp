#include "HartiganMaster.hpp"

RcppExport SEXP run(SEXP &numberOfClusters, SEXP &numberOfTimes, SEXP &dataset) {

  int nrOfClusters = Rcpp::as<int>(numberOfClusters);
  Rcpp::NumericMatrix proxyDataset(dataset);
  //reuses memory and avoids extra copy
  arma::mat points(proxyDataset.begin(), proxyDataset.nrow(), proxyDataset.ncol(), false);

  //random assignment into clusters
  int seed = time(NULL);
  srand(seed);

  std::vector<int> fits;
  fits.reserve(points.n_rows);

  for(int i = 0; i < points.n_rows; i++) fits[i] = rand()%nrOfClusters;

  //Hartigan algorithm
  Hartigan algorithm(nrOfClusters, 0.0001, fits, points);

  algorithm.loop();

  return Rcpp::List::create(fits);
}
