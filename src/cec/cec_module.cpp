#include "cec_module.hpp"

namespace gmum {

  CEC* CEC__new(SEXP args) {

    Rcpp::List list(args);
    const Params params = processArguments(list);
    verifyParams(params);
    return findBestCEC(params);
  }



  Params processArguments(const Rcpp::List list) {
    Params params;

    if(list.containsElementNamed(CONST::dataset)) {
      params.datasetSet = true;
      Rcpp::NumericMatrix proxyDataset = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::dataset]);
      //reuses memory and avoids extra copy
      boost::shared_ptr<const arma::mat> points(new arma::mat(proxyDataset.begin(), proxyDataset.nrow(),
							      proxyDataset.ncol(), false));
      params.dataset = points;
    } else params.datasetSet = false;

    if(list.containsElementNamed(CONST::killThreshold))
      params.killThreshold = Rcpp::as<double>(list[CONST::killThreshold]);
    else params.killThreshold = CONST::killThresholdInit;

    if(list.containsElementNamed(CONST::clusters)) {

      Rcpp::List desc = Rcpp::as<Rcpp::List>(list[CONST::clusters]);

      for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it) {
	Rcpp::List list = Rcpp::as<Rcpp::List>(*it);

	boost::shared_ptr<ClusterParams> cluster;
	if(!list.containsElementNamed(CONST::CLUSTERS::type)) {

	  cluster.reset(new ClusterParams());
	  cluster->type = noType;

	} else {

	  std::string typeStr = Rcpp::as<std::string>(list[CONST::CLUSTERS::type]);

	  if(typeStr.compare(CONST::CLUSTERS::standard)==0) {

	    cluster.reset(new ClusterParams());
	    cluster->type = standard;

	  } else if(typeStr.compare(CONST::CLUSTERS::full)==0) {

	    ClusterFullParams *ptr = new ClusterFullParams();
	    ptr->type = full;
	   
	    if(list.containsElementNamed(CONST::CLUSTERS::covMat)) {
	      ptr->covMatSet = true;
	      Rcpp::NumericMatrix temp = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::CLUSTERS::covMat]);
	      ptr->covMat = arma::mat(temp.begin(), temp.nrow(), temp.ncol());
	    } else ptr->covMatSet = false;

	    cluster.reset(ptr);

	  } else if(typeStr.compare(CONST::CLUSTERS::fsphere)==0) {

	    ClusterFsphereParams *ptr = new ClusterFsphereParams();
	    ptr->type = fsphere;

	    if(list.containsElementNamed(CONST::CLUSTERS::radius)) {
	      ptr->radiusSet = true;
	      ptr->radius = Rcpp::as<double>(list[CONST::CLUSTERS::radius]);
	    } else ptr->radiusSet = false;

	    cluster.reset(ptr);

	  } else if(typeStr.compare(CONST::CLUSTERS::sphere)==0) {
	    cluster.reset(new ClusterParams());
	    cluster->type = sphere;
	  } else if(typeStr.compare(CONST::CLUSTERS::diagonal)==0) {
	    cluster.reset(new ClusterParams());
	    cluster->type = diagonal;
	  } else {
	    cluster.reset(new ClusterParams());
	    cluster->type = noType;
	  }
	}

	params.clusters.push_back(cluster);
      }
    }

    params.nrOfClusters = CONST::nrOfClustersInit;
    if(list.containsElementNamed(CONST::nrOfClusters))
      params.nrOfClusters = Rcpp::as<int>(list[CONST::nrOfClusters]);
    else if(params.clusters.size() > 0) params.nrOfClusters = params.clusters.size();

    if(list.containsElementNamed(CONST::logClusters) && Rcpp::as<bool>(list(CONST::logClusters)))
      params.logNrOfClusters = true;
    else params.logNrOfClusters = false;

    if(list.containsElementNamed(CONST::energy) && Rcpp::as<bool>(list(CONST::energy)))
      params.logEnergy = true;
    else params.logEnergy = false;

    params.nstart = CONST::nstartInit;
    if(list.containsElementNamed(CONST::nstart))
      params.nstart = Rcpp::as<int>(list[CONST::nstart]);

    params.assignmentType = CONST::defaultAssignment;
    if(list.containsElementNamed(CONST::CLUSTERS::init)) {
      std::string init = Rcpp::as<std::string>(list(CONST::CLUSTERS::init));
      if(init.compare(CONST::CLUSTERS::random)==0) params.assignmentType = random;
      else if(init.compare(CONST::CLUSTERS::kmeanspp)==0) params.assignmentType = kmeanspp;
      else if(init.compare(CONST::CLUSTERS::centroids)==0) {
	params.assignmentType = centroids;
	if(list.containsElementNamed(CONST::centroidsList)) {
	  params.centroidsSet = true;
	  Rcpp::List desc = Rcpp::as<Rcpp::List>(list[CONST::centroidsList]);

	  for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
	    params.centroids.push_back(Rcpp::as<std::vector<double> >(*it));

	} else params.centroidsSet = false;
    }

    if(params.clusters.size() > 0) params.clusterType = mix;
    else {
      params.clusterType = standard;
      if(list.containsElementNamed(CONST::CLUSTERS::type)) {
	std::string type = Rcpp::as<std::string>(list[CONST::CLUSTERS::type]);
	if(type.compare(CONST::CLUSTERS::standard)==0) {
	  //nothing but recognises
	} else if(type.compare(CONST::CLUSTERS::full)==0) {
	  params.clusterType = full;
	} else if(type.compare(CONST::CLUSTERS::diagonal)==0) {
	  params.clusterType = diagonal;
	} else if(type.compare(CONST::CLUSTERS::sphere)==0) {
	  params.clusterType = sphere;
	} else if(type.compare(CONST::CLUSTERS::fsphere)==0) {
	  params.clusterType = fsphere;
	} else params.clusterType = noType;
      }

      if(list.containsElementNamed(CONST::CLUSTERS::covMat)) {
	params.covMatSet = true;
	Rcpp::NumericMatrix covMatProxy = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::CLUSTERS::covMat]);
	params.covMat = arma::mat(covMatProxy.begin(), covMatProxy.nrow(), covMatProxy.ncol());
      } else params.covMatSet = false;

      if(list.containsElementNamed(CONST::CLUSTERS::radius)) {
	params.radiusSet = true;
	params.radius = Rcpp::as<double>(list[CONST::CLUSTERS::radius]);
      } else params.radiusSet = false;
    }

    return params;
  }



  void verifyParams(const Params params) {

    if(!params.datasetSet)
      Rcpp::stop(CONST::ERRORS::datasetReq);

    if(params.nstart <= 0)
      Rcpp::stop(CONST::ERRORS::nstartsPositive);

    if(params.nrOfClusters <= 0)
      Rcpp::stop(CONST::ERRORS::nrOfClustersPositive);

    if(params.dataset->n_rows < params.nrOfClusters) 
      Rcpp::stop(CONST::ERRORS::datasetSize);

    if(params.killThreshold > 1.0/params.nrOfClusters)
      Rcpp::stop(CONST::ERRORS::killThresholdSize);

    if(params.assignmentType == noAssignment)
      Rcpp::stop(CONST::ERRORS::assignmentError);

    if(params.assignmentType == centroids && params.centroids.size() != params.nrOfClusters)
      Rcpp::stop(CONST::ERRORS::centroidsError);

    switch(params.clusterType) {
    case full:
      if(!params.covMatSet) Rcpp::stop(CONST::ERRORS::covMatReq);
      break;
    case fsphere:
      if(!params.radiusSet) Rcpp::stop(CONST::ERRORS::radiusReq);
      break;
    case noType:
      Rcpp::stop(CONST::ERRORS::clusterRecError);
      break;
    case mix:
      for(std::list<boost::shared_ptr<ClusterParams> >::const_iterator it = params.clusters.begin();
	  it != params.clusters.end(); ++it)
	switch((*it)->type) {
	case full: {
	  ClusterFullParams &ptr = static_cast<ClusterFullParams&>(*(*it));
	  if(!ptr.covMatSet) Rcpp::stop(CONST::ERRORS::covMatReq);
	  break;
	}
	case fsphere: {
	  ClusterFsphereParams &ptr = static_cast<ClusterFsphereParams&>(*(*it));
	  if(!ptr.radiusSet) Rcpp::stop(CONST::ERRORS::radiusReq);
	  break;
	}
	case noType:
	  Rcpp::stop(CONST::ERRORS::clusterRecError);
	}
    }
  }



  CEC *findBestCEC(const Params params) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(params.logNrOfClusters, params.logEnergy));

    Assignment *assignmentType;
    switch(params.assignmentType) {
    case random:
      assignmentType = new RandomAssignment(*points, params.nrOfClusters);
      break;
    case kmeanspp:
      assignmentType = new KmeansppAssignment(*points, params.nrOfClusters);
      break;
    case centroids:
      assignmentType = new CentroidsAssignment(*points, params.centroids);
      break;
    }

    assignment->resize(params.dataset.n_rows);
    assignmentType(assignment);

    CEC *currentCEC = NULL;

    try {
      currentCEC = new CEC(hartigan, assignment, params);

      currentCEC->loop();

      for(int i=1; i<nstart; ++i) {

	assignmentType(assignment);
	CEC *nextCEC = new CEC(hartigan, assignment, params);
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


  
  arma::mat getDataSet(CEC* cec) {
    return *(cec->getPtrToPoints());
  }

  unsigned int getNstart(CEC* cec) {
    return nstart;
  }
}
