#ifndef CECCONFIGURATION_HPP_
#define CECCONFIGURATION_HPP_

#include "params.hpp"
#include <Rcpp.h>

class cecConfiguration {
private:
    gmum::Params params;
public:
	cecConfiguration();
    gmum::Params getParams();
	void setDataSet(const Rcpp::NumericMatrix);
	void setNrOfClusters(const unsigned int);
	void setCentroids(const Rcpp::List);
	void setNstart(const unsigned int);
	void setMethodType(const std::string);
	void setMethodInit(const std::string);
	void setR(const double);
	void setCov(const Rcpp::NumericMatrix);
	void setMix(const Rcpp::List);
	void setFunction(const std::string);
	void setEps(const double);
	void setItmax(const unsigned int);
	void setLogEnergy(bool);
	void setNCluster(bool);
	void setIters(bool);
};

#endif /* CECCONFIGURATION_HPP_ */
