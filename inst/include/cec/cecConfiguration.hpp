#ifndef CECCONFIGURATION_HPP_
#define CECCONFIGURATION_HPP_

#include "cec.hpp"

namespace gmum {

class cecConfiguration {
private:
	Params params;
public:
	cecConfiguration();
	Params getParams();
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

} /* namespace gmum */

#endif /* CECCONFIGURATION_HPP_ */
