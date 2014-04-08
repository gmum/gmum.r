#include "HartiganMaster.h"

RcppExport SEXP run(SEXP args) {

  BEGIN_RCPP;
  /*
   * arguments processing 
   */
  Rcpp::List list(args);

  Rcpp::NumericMatrix proxyDataset = Rcpp::as<Rcpp::NumericMatrix>(list["dataset"]);
  //reuses memory and avoids extra copy
  arma::mat points(proxyDataset.begin(), proxyDataset.nrow(), proxyDataset.ncol(), false);

  float killThreshold = 0.001;
  if(list.containsElementNamed("killThreshold"))
    killThreshold = Rcpp::as<float>(list["killThreshold"]);

  std::list<Rcpp::List> clusters;
  initClusters(clusters, list);

  //vectors init
  std::vector<ClusterType> type;
  std::vector<arma::mat> covMat;
  std::vector<float> radius;
  initVectors(type, covMat, radius, clusters);

  //random assignment into clusters
  std::vector<unsigned int> assignment;
  initAssignRandom(assignment, points.n_rows, clusters.size());
  
  //CEC init
  CEC *cec;
  Hartigan hartigan;
  cec = new CEC(points, assignment, killThreshold, &hartigan,
		  type, radius, covMat);

  cec->loop();

  return Rcpp::List::create(assignment);
  END_RCPP;
}

void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list) {

  if(list.containsElementNamed("K")) {

    unsigned int nrOfClusters = Rcpp::as<int>(list["K"]);
    for(unsigned int i = 0; i < nrOfClusters; ++i)
      clusters.push_back(Rcpp::List::create(Rcpp::Named("type") = "usual"));

  } else if(list.containsElementNamed("clusters")) {

    Rcpp::List desc = Rcpp::as<Rcpp::List>(list["clusters"]);
    
    for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
      clusters.push_back(Rcpp::as<Rcpp::List>(*it));
  }
}

void initVectors(std::vector<ClusterType> &type, 
		 std::vector<arma::mat> &covMat,
		 std::vector<float> &radius,
		 std::list<Rcpp::List> clusters) {

  type.resize(clusters.size());
  covMat.resize(clusters.size());
  radius.resize(clusters.size());

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


void initAssignRandom(std::vector<unsigned int> &assignment,
		      unsigned int nrOfPoints,
		      unsigned int nrOfClusters) {
  int seed = time(NULL);
  srand(seed);
  assignment.reserve(nrOfPoints);

  for(unsigned int i = 0; i < nrOfPoints; i++) assignment.push_back(rand()%nrOfClusters);
}
