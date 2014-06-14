#include "cec_module.hpp"

namespace gmum {

  CEC* CEC__new(SEXP args) {

    /*
     * arguments processing 
     */
    Rcpp::List list(args);

    if(!list.containsElementNamed(CONST::dataset))
      Rcpp::stop("dataset is required!");

    Rcpp::NumericMatrix proxyDataset = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::dataset]);

    //reuses memory and avoids extra copy
    boost::shared_ptr<const arma::mat> points(new arma::mat(proxyDataset.begin(), proxyDataset.nrow(),
						      proxyDataset.ncol(), false));

    /*
     * read predefined clusters or creates standard description
     * in standard number or number specified by user
     */
    std::list<Rcpp::List> clusters;
    initClusters(clusters, list);

    if(points->n_rows < clusters.size()) 
      Rcpp::stop("Size of dataset cannot be less than number of clusters!");

    double killThreshold = CONST::killThresholdInit;
    if(list.containsElementNamed(CONST::killThreshold)) {

      killThreshold = Rcpp::as<double>(list[CONST::killThreshold]);
      if(killThreshold > 1.0/clusters.size())
	Rcpp::stop(std::string(CONST::killThreshold)+" is too hight");
    }

    //vectors init
    std::vector<ClusterType> type;
    std::vector<arma::mat> covMat;
    std::vector<double> radius;
    initVectors(type, covMat, radius, clusters);

    //logging options
    bool logNrOfClusters, logEnergy;

    if(list.containsElementNamed(CONST::nclusters) && Rcpp::as<bool>(list(CONST::nclusters)))
      logNrOfClusters = true;
    else logNrOfClusters = false;

    if(list.containsElementNamed(CONST::energy) && Rcpp::as<bool>(list(CONST::energy)))
      logEnergy = true;
    else logEnergy = false;

    //number of starts
    nstart = CONST::nstartInit;
    if(list.containsElementNamed(CONST::nstart)) {
      unsigned int temp = Rcpp::as<unsigned int>(list[CONST::nstart]);
      if(temp == 0 || temp > 1e+4) Rcpp::stop(std::string(CONST::nstart)+" is out of bounds");
      else nstart = temp;
    }

    //type of random assignment
    Assignment assignmentType = kmeanspp;
    if(list.containsElementNamed(CONST::CLUSTERS::init)) {
      std::string init = Rcpp::as<std::string>(list(CONST::CLUSTERS::init));
      if(init.compare(CONST::CLUSTERS::random)==0) assignmentType = random;
      else if(init.compare(CONST::CLUSTERS::kmeanspp)==0) assignmentType = kmeanspp;
      else Rcpp::stop(std::string("cannot recognize ")+init);
    }

    //CEC creation
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(logNrOfClusters, logEnergy));

    randomAssignment(assignmentType, *assignment, *points, clusters.size());

    CEC *currentCEC = NULL;

    try {
      currentCEC = new CEC(points, assignment, hartigan,
			   killThreshold, type, radius, covMat);

      currentCEC->loop();

      for(int i=1; i<nstart; ++i) {

	randomAssignment(assignmentType, *assignment, *points, clusters.size());
	CEC *nextCEC = new CEC(points, assignment, hartigan,
			       killThreshold, type, radius, covMat);
	nextCEC->loop();

	if(nextCEC->entropy() < currentCEC->entropy()) {
	  delete currentCEC;
	  currentCEC = nextCEC;
	}
      }
    } catch(std::exception e) {
      Rcpp::stop(std::string("exception ")+e.what()+" caught in CEC_new");
    }

    return currentCEC;
  }



  void initClusters(std::list<Rcpp::List> &clusters, Rcpp::List &list) {

    if(list.containsElementNamed(CONST::clusters)) {

      Rcpp::List desc = Rcpp::as<Rcpp::List>(list[CONST::clusters]);

      for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
	clusters.push_back(Rcpp::as<Rcpp::List>(*it));

    } else {

      unsigned int nrOfClusters = CONST::nrOfClustersInit;
      if(list.containsElementNamed(CONST::nrOfClusters)) {
	int temp = Rcpp::as<int>(list[CONST::nrOfClusters]);
	if(temp <= 0)
	  Rcpp::stop("Number of clusters should be a positive integer!");
	else nrOfClusters = temp;
      }
    
      Rcpp::List typeList = Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = 
					       CONST::CLUSTERS::standard);
      if(list.containsElementNamed(CONST::CLUSTERS::type)) {
	std::string type = Rcpp::as<std::string>(list[CONST::CLUSTERS::type]);

	if(type.compare(CONST::CLUSTERS::standard)==0) {
	  //nothing but recognises
	} else if(type.compare(CONST::CLUSTERS::full)==0) {

	  if(!list.containsElementNamed(CONST::CLUSTERS::covMat))
	    Rcpp::stop("cavariance matrix required");

	  Rcpp::NumericMatrix covMat = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::CLUSTERS::covMat]);

	  typeList = Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = 
					CONST::CLUSTERS::full,
					Rcpp::Named(CONST::CLUSTERS::covMat) = covMat);

	} else if(type.compare(CONST::CLUSTERS::diagonal)==0) {

	  typeList = Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = 
					CONST::CLUSTERS::diagonal);

	} else if(type.compare(CONST::CLUSTERS::sphere)==0) {

	  typeList = Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = 
					CONST::CLUSTERS::sphere);

	} else if(type.compare(CONST::CLUSTERS::fsphere)==0) {

	  if(!list.containsElementNamed(CONST::CLUSTERS::radius))
	    Rcpp::stop("radius required");

	  double radius = Rcpp::as<double>(list[CONST::CLUSTERS::radius]);
	  typeList = Rcpp::List::create(Rcpp::Named(CONST::CLUSTERS::type) = 
					CONST::CLUSTERS::fsphere,
					Rcpp::Named(CONST::CLUSTERS::radius) = radius);

	} else Rcpp::stop("cannot recognise cluster type");

      }

      for(unsigned int i = 0; i < nrOfClusters; ++i)
	clusters.push_back(typeList);
    }
  }



  void initVectors(std::vector<ClusterType> &type, 
		   std::vector<arma::mat> &covMat,
		   std::vector<double> &radius,
		   std::list<Rcpp::List> &clusters) {

    type.resize(clusters.size());
    covMat.resize(clusters.size());
    radius.resize(clusters.size());

    int i = 0;
    for(std::list<Rcpp::List>::iterator it = clusters.begin();
	it != clusters.end(); ++it, ++i) {

      std::string typeStr = Rcpp::as<std::string>((*it)[CONST::CLUSTERS::type]);

      if(typeStr.compare(CONST::CLUSTERS::standard)==0) type[i] = standard;
      else if(typeStr.compare(CONST::CLUSTERS::full)==0) {

	type[i] = full;
	Rcpp::NumericMatrix temp = Rcpp::as<Rcpp::NumericMatrix>((*it)[CONST::CLUSTERS::full]);
	covMat[i] = arma::mat(temp.begin(), temp.nrow(), temp.ncol());

      } else if(typeStr.compare(CONST::CLUSTERS::fsphere)==0) {

	type[i] = fsphere;
	radius[i] = Rcpp::as<double>((*it)[CONST::CLUSTERS::radius]);

      } else if(typeStr.compare(CONST::CLUSTERS::sphere)==0) type[i] = sphere;
      else if(typeStr.compare(CONST::CLUSTERS::diagonal)==0) type[i] = diagonal;
      else Rcpp::stop("you must specify cluster's type");

    }
  }



  void randomAssignment(Assignment assignmentType, std::vector<unsigned int> &assignment,
			const arma::mat &points, int nrOfClusters) {

    assignment.resize(points.n_rows);

      switch(assignmentType) {
      case random:
	initAssignRandom(assignment, nrOfClusters);
	break;
      case kmeanspp:
	initAssignKmeanspp(assignment, points, nrOfClusters);
	break;
      }
  }



  std::list<double> CECpredict(CEC* cec, std::vector<double> vec, bool general) {
    arma::rowvec x = arma::conv_to<arma::rowvec>::from(vec);
    std::list<double> out;

    if(general)
      for(int i=0; i<cec->clusters.size(); ++i) out.push_back(cec->clusters[i]->predict(x));

    return out;
  }



  unsigned int CECpredict(CEC *cec, std::vector<double> vec) {
    arma::rowvec x = arma::conv_to<arma::rowvec>::from(vec);

    int assign = 0;
    double minEntropyChange = std::numeric_limits<double>::max();
  
    for(int i=0; i<cec->clusters.size(); ++i) {

      boost::shared_ptr<Cluster> oldCluster = cec->clusters[i];
      boost::shared_ptr<Cluster> newCluster = cec->clusters[i]->addPoint(x);
      double entropyChange = newCluster->entropy() - oldCluster->entropy();

      if(entropyChange < minEntropyChange) {
	minEntropyChange = entropyChange;
	assign = i;
      }
    }

    return assign;
  }
  //should be removed 
  void plot(CEC* cec) {
    //  Rcpp::Language call("plot",Rcpp::List::create(Rcpp::Named("x")=*(cec->getPtrToPoints()),			  Rcpp::Named("col") = *(cec->getPtrToAssignement())));
        // Rcpp::Language call("plot",*(cec->getPtrToPoints()),Rcpp::Named("col")=cec->getAssignment());
     Rcpp::Language call("plot",*(cec->getPtrToPoints()));
    call.eval();
  }
  
  arma::mat getDataSet(CEC* cec) {
    return *(cec->getPtrToPoints());
  }

  unsigned int getNstart(CEC* cec) {
    return nstart;
  }
}
