#ifndef CECCONFIGURATION_HPP
#define CECCONFIGURATION_HPP

#include "params.hpp"
#include <Rcpp.h>

class cecConfiguration {
private:
	gmum::Params params;
public:
	cecConfiguration();
	gmum::Params getParams();
	void setParams(gmum::Params params);
	void setDataSet(const Rcpp::NumericMatrix);
	void setEps(const double);
	void setMix(const Rcpp::List);
	void setNrOfClusters(const unsigned int);
	void setLogEnergy(bool);
	void setLogCluster(bool);
	void setNstart(const unsigned int);
	void setCentroids(const Rcpp::List);
	void setMethodInit(const std::string);
	void setMethodType(const std::string);
	void setCov(const Rcpp::NumericMatrix);
	void setR(const double);
	void setFunction(const std::string);
	void setItmax(const unsigned int);
	void setIters(bool);
};

#endif /* CECCONFIGURATION_HPP */
