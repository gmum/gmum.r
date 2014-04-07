#include "HartiganMaster.hpp"

RcppExport SEXP run(SEXP args) {

  BEGIN_RCPP
  Rcpp::List list(args);

  Rcpp::NumericMatrix proxyDataset = Rcpp::as<Rcpp::NumericMatrix>(list["dataset"]);
  //reuses memory and avoids extra copy
  arma::mat points(proxyDataset.begin(), proxyDataset.nrow(), proxyDataset.ncol(), false);

  float killThreshold = 0.001;
  if(list.containsElementNamed("killThreshold"))
    killThreshold = Rcpp::as<float>(list["killThreshold"]);

  unsigned int nrOfClusters = 0;
  bool customClusters = false;
  std::list<Rcpp::List> clusters;

  if(list.containsElementNamed("K"))

    nrOfClusters = Rcpp::as<int>(list["K"]);

  else if(list.containsElementNamed("clusters")) {

    customClusters = true;
    Rcpp::List desc = Rcpp::as<Rcpp::List>(list["clusters"]);
    
    for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
      clusters.push_back(Rcpp::as<Rcpp::List>(*it));
    
    nrOfClusters = clusters.size();
  }

  //random assignment into clusters
  int seed = time(NULL);
  srand(seed);

  std::vector<unsigned int> fits;
  fits.reserve(points.n_rows);

  for(unsigned int i = 0; i < points.n_rows; i++) fits[i] = rand()%nrOfClusters;
  
  //vectors init
  std::vector<ClusterType> type;
  std::vector<arma::mat> covMat;
  std::vector<float> radius;
  if(customClusters) {
    type.reserve(nrOfClusters);
    covMat.reserve(nrOfClusters);
    radius.reserve(nrOfClusters);

    int i = 0;
    for(std::list<Rcpp::List>::iterator it = clusters.begin();
	it != clusters.end(); ++it, ++i) {

      std::string typeStr = Rcpp::as<std::string>((*it)["type"]);

      if(typeStr.compare("usual")==0) type[i] = usual;
      else if(typeStr.compare("covMat")==0) {

	type[i] = covMatrix;
	Rcpp::NumericMatrix temp = Rcpp::as<Rcpp::NumericMatrix>((*it)["covMat"]);
	covMat[i] = arma::mat(temp.begin(), temp.nrow(), temp.ncol());

      } else if(typeStr.compare("constRadius")==0) {

	type[i] = constRadius;
	radius[i] = Rcpp::as<float>((*it)["radius"]);

      } else if(typeStr.compare("spherical")==0) type[i] = spherical;
      else if(typeStr.compare("diagonal")==0) type[i] = diagonal;
      //else must not happen
    }
  }
  
  //CEC init
   CEC *cec;
  Hartigan hartigan;
  if(customClusters)
    cec = new CEC(points, fits, killThreshold, &hartigan,
	      type, radius, covMat);
  else
    cec = new CEC(points, fits, killThreshold, &hartigan,
	      nrOfClusters);

	      cec->loop();

  return Rcpp::List::create(fits);
  END_RCPP
}
