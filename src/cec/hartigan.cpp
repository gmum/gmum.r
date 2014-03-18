#include "hartigan.hpp"

RcppExport SEXP hartigan(SEXP dataset, SEXP NrClusters) {
  using namespace Rcpp;
  NumericMatrix data(dataset);
  int nrClusters = NumericVector(NrClusters)[0];
  
  std::vector<int> cluster(data.nrow());
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0,nrClusters);
  for(int i=0; i<data.nrow(); i++) cluster[i] = distribution(generator);

  std::vector<double> energy(nrClusters);
  for(int k=0; k<nrClusters; k++) energy[k] = calcEnergy(data, k);

  int i = 0;
  do {
    int k = cluster[i];
    for(int j=0; j<nrClusters; j++) {
      if(j==k) continue;
      double energyChange = calcChange();
      if(energyChange < 0) cluster[i] = [j];
    }
  } while(true);
  return data;
}
