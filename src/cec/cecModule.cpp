#include "cecModule.hpp"
namespace gmum {

  CEC* CEC__new(SEXP args) {
    Rcpp::List list(args);
    const Params params = processArguments(list);
    verifyParams(params);
    return findBestCEC(params);
  }

  Params processArguments(const Rcpp::List &list) {
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
	  cluster->type = knoType;

	} else {

	  std::string typeStr = Rcpp::as<std::string>(list[CONST::CLUSTERS::type]);

	  if(typeStr.compare(CONST::CLUSTERS::standard)==0) {

	    cluster.reset(new ClusterParams());
	    cluster->type = kstandard;

	  } else if(typeStr.compare(CONST::CLUSTERS::full)==0) {

	    ClusterFullParams *ptr = new ClusterFullParams();
	    ptr->type = kfull;
	   
	    if(list.containsElementNamed(CONST::CLUSTERS::covMat)) {
	      ptr->covMatSet = true;
	      Rcpp::NumericMatrix temp = Rcpp::as<Rcpp::NumericMatrix>(list[CONST::CLUSTERS::covMat]);
	      ptr->covMat = arma::mat(temp.begin(), temp.nrow(), temp.ncol());
	    } else ptr->covMatSet = false;

	    cluster.reset(ptr);

	  } else if(typeStr.compare(CONST::CLUSTERS::fsphere)==0) {

	    ClusterFsphereParams *ptr = new ClusterFsphereParams();
	    ptr->type = kfsphere;

	    if(list.containsElementNamed(CONST::CLUSTERS::radius)) {
	      ptr->radiusSet = true;
	      ptr->radius = Rcpp::as<double>(list[CONST::CLUSTERS::radius]);
	    } else ptr->radiusSet = false;

	    cluster.reset(ptr);

	  } else if(typeStr.compare(CONST::CLUSTERS::sphere)==0) {
	    cluster.reset(new ClusterParams());
	    cluster->type = ksphere;
	  } else if(typeStr.compare(CONST::CLUSTERS::diagonal)==0) {
	    cluster.reset(new ClusterParams());
	    cluster->type = kdiagonal;
          } else if(typeStr.compare(CONST::CLUSTERS::custom)==0) {
            ClusterCustomParams *ptr = new ClusterCustomParams();
            ptr->type = kcustom;
            if (list.containsElementNamed(CONST::CLUSTERS::functionName)==0) {
              ptr->functionNameSet = true;
              ptr->functionName = Rcpp::as<std::string>(
              list[CONST::CLUSTERS::functionName]); 
            } else {
              ptr->functionNameSet = false;
            }
            cluster.reset(ptr);
	  } else {
	    cluster.reset(new ClusterParams());
	    cluster->type = knoType;
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
      if(init.compare(CONST::CLUSTERS::random)==0) params.assignmentType = krandom;
      else if(init.compare(CONST::CLUSTERS::kmeanspp)==0) params.assignmentType = kkmeanspp;
      else if(init.compare(CONST::CLUSTERS::centroids)==0) {
	params.assignmentType = kcentroids;
	if(list.containsElementNamed(CONST::centroidsList)) {
	  params.centroidsSet = true;
	  Rcpp::List desc = Rcpp::as<Rcpp::List>(list[CONST::centroidsList]);

	  for(Rcpp::List::iterator it = desc.begin(); it != desc.end(); ++it)
	    params.centroids.push_back(Rcpp::as<std::vector<double> >(*it));

	} else params.centroidsSet = false;
      }
    }

    if(params.clusters.size() > 0) params.clusterType = kmix;
    else {
      params.clusterType = kstandard;
      if(list.containsElementNamed(CONST::CLUSTERS::type)) {
	std::string type = Rcpp::as<std::string>(list[CONST::CLUSTERS::type]);
        //Rcpp::stop(type);
        if(type.compare(CONST::CLUSTERS::standard)==0) {
	  //nothing but recognises
	} else if(type.compare(CONST::CLUSTERS::full)==0) {
	  params.clusterType = kfull;
	} else if(type.compare(CONST::CLUSTERS::diagonal)==0) {
	  params.clusterType = kdiagonal;
	} else if(type.compare(CONST::CLUSTERS::sphere)==0) {
	  params.clusterType = ksphere;
	} else if(type.compare(CONST::CLUSTERS::fsphere)==0) {
	  params.clusterType = kfsphere;
        } else if(type.compare(CONST::CLUSTERS::custom)==0) {
          params.clusterType = kcustom;
	} else params.clusterType = knoType;
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
      
      if(list.containsElementNamed(CONST::CLUSTERS::functionName)) {
        params.functionNameSet = true;
        params.functionName = Rcpp::as<std::string>(list[CONST::CLUSTERS::functionName]);
      } else {
        params.functionNameSet = false;
      }
    } 
    return params;
  }



  void verifyParams(const Params &params) {

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

    if(params.assignmentType == knoAssignment)
      Rcpp::stop(CONST::ERRORS::assignmentError);

    if(params.assignmentType == kcentroids && params.centroids.size() != params.nrOfClusters)
      Rcpp::stop(CONST::ERRORS::centroidsError);

    switch(params.clusterType) {
    case kfull:
      if(!params.covMatSet) Rcpp::stop(CONST::ERRORS::covMatReq);
      break;
    case kfsphere:
      if(!params.radiusSet) Rcpp::stop(CONST::ERRORS::radiusReq);
      break;
    case knoType:
      Rcpp::stop(CONST::ERRORS::clusterRecError);
      break;
    case kcustom:
      if(!params.functionNameSet) Rcpp::stop(CONST::ERRORS::functionNameReq);
      break;
    case kmix:
      BOOST_FOREACH(boost::shared_ptr<ClusterParams> cluster, params.clusters) {
	switch(cluster->type) {
	case kfull: {
	  ClusterFullParams &ptr = static_cast<ClusterFullParams&>(*cluster);
	  if(!ptr.covMatSet) Rcpp::stop(CONST::ERRORS::covMatReq);
	  break;
	}
	case kfsphere: {
	  ClusterFsphereParams &ptr = static_cast<ClusterFsphereParams&>(*cluster);
	  if(!ptr.radiusSet) Rcpp::stop(CONST::ERRORS::radiusReq);
	  break;
	}
	case knoType:
	  Rcpp::stop(CONST::ERRORS::clusterRecError);
        case kcustom:
          ClusterCustomParams &ptr = static_cast<ClusterCustomParams&>(*cluster);
          if(!ptr.functionNameSet) Rcpp::stop(CONST::ERRORS::functionNameReq);
          break;
	}
      }
      
    }
  }



  CEC *findBestCEC(const Params &params) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(params.logNrOfClusters, params.logEnergy));

    Assignment *assignmentType;
    switch(params.assignmentType) {
    case krandom:
      assignmentType = new RandomAssignment(*(params.dataset), params.nrOfClusters);
      break;
    case kkmeanspp:
      assignmentType = new KmeansppAssignment(*(params.dataset), params.nrOfClusters);
      break;
    case kcentroids:
      assignmentType = new CentroidsAssignment(*(params.dataset), params.centroids);
      break;
    }

    assignment->resize(params.dataset->n_rows);
    (*assignmentType)(*assignment);

    CEC *currentCEC = NULL;
    try {
      currentCEC = new CEC(hartigan, assignment, params);

      currentCEC->loop();

      for(int i=1; i<nstart; ++i) {

	(*assignmentType)(*assignment);
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

    delete assignmentType;
    return currentCEC;
  }


  
  arma::mat getDataSet(CEC* cec) {
    return *(cec->getPtrToPoints());
  }

  unsigned int getNstart(CEC* cec) {
    return nstart;
  }
}
