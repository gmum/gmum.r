#include "cec_module.hpp"
#include "random_assignment.hpp"

CEC* CEC__new(SEXP args) {

  /*
   * arguments processing 
   */
  Rcpp::List list(args);

  Rcpp::NumericMatrix proxyDataset = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::dataset]);
  //reuses memory and avoids extra copy
  boost::shared_ptr<arma::mat> points(new arma::mat(proxyDataset.begin(), proxyDataset.nrow(),
						   proxyDataset.ncol(), false));

  float killThreshold = 0.001;
  if(list.containsElementNamed(CONST::killThreshold))
    killThreshold = Rcpp::as<float>(list[CONST::killThreshold]);

  std::list<Rcpp::List> clusters;
  initClusters(clusters, list);

  //vectors init
  std::vector<ClusterType> type;
  std::vector<arma::mat> covMat;
  std::vector<float> radius;
  initVectors(type, covMat, radius, clusters);

  //random assignment into clusters
  boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
  initAssignRandom(*assignment, points->n_rows, clusters.size());
  
  //CEC creation
  boost::shared_ptr<Hartigan> hartigan(new Hartigan());
  return new CEC(points, assignment, hartigan,
		 killThreshold, type, radius, covMat);
}

void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list) {

  if(list.containsElementNamed(CONST::nrOfClusters)) {

    unsigned int nrOfClusters = Rcpp::as<int>(list[CONST::nrOfClusters]);
    for(unsigned int i = 0; i < nrOfClusters; ++i)
      clusters.push_back(Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = CONST::CLUSTERS::usual));

  } else if(list.containsElementNamed(CONST::clusters)) {

    Rcpp::List desc = Rcpp::as<Rcpp::List>(list[CONST::clusters]);
    
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

    std::string typeStr = Rcpp::as<std::string>((*it)[CONST::CLUSTERS::type]);

    if(typeStr.compare(CONST::CLUSTERS::usual)==0) type[i] = usual;
    else if(typeStr.compare(CONST::CLUSTERS::covMat)==0) {

      type[i] = covMatrix;
      Rcpp::NumericMatrix temp = Rcpp::as<Rcpp::NumericMatrix>((*it)[CONST::CLUSTERS::covMat]);
      covMat[i] = arma::mat(temp.begin(), temp.nrow(), temp.ncol());

    } else if(typeStr.compare(CONST::CLUSTERS::constRadius)==0) {

      type[i] = constRadius;
      radius[i] = Rcpp::as<float>((*it)[CONST::CLUSTERS::radius]);

    } else if(typeStr.compare(CONST::CLUSTERS::spherical)==0) type[i] = spherical;
    else if(typeStr.compare(CONST::CLUSTERS::diagonal)==0) type[i] = diagonal;
    //else must not happen
  }
}

unsigned int CECpredict(CEC *cec, std::vector<float> vec) {
  arma::rowvec x = arma::conv_to<arma::rowvec>::from(vec);

  int assign = 0;
  float minEntropyChange = std::numeric_limits<float>::max();
  
  for(int i=0; i<cec->clusters.size(); ++i) {

    Cluster &oldCluster = cec->clusters[i];
    Cluster newCluster = cec->clusters[i].addPoint(x);
    float entropyChange = newCluster.entropy() - oldCluster.entropy();

    if(entropyChange < minEntropyChange) {
      minEntropyChange = entropyChange;
      assign = i;
    }
  }

  return assign;
}
