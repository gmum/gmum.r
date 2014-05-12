#ifndef RGNG_SERVER_H

#define RGNG_SERVER_H


#include "GNGServer.h"
#include "GNGConfiguration.h"
#include <RcppArmadillo.h>
#include <armadillo>


/*
* This class defines proxy to GNGServer. In most cases it simply calls methods from GNGServer
* or just assembly responses into R structures
*/
class RcppGNGServer{

	GNGServer * server;

public:


	RcppGNGServer(GNGConfiguration config){
		GNGServer::setConfiguration(config);
		server = &GNGServer::getInstance();
	}


	void insertExamples(Rcpp::NumericMatrix & ex){



		arma::mat points(ex.begin(), ex.nrow(), ex.ncol(), false);
		arma::inplace_trans( points, "lowmem");
	}



};

#endif
