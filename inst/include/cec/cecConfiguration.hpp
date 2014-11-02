#ifndef CECCONFIGURATION_HPP_
#define CECCONFIGURATION_HPP_

#include "cec.hpp"
#include "const.hpp"

namespace gmum {

class cecConfiguration {
private:
	Params params;
public:
	cecConfiguration();
	Params getParams();
	void setX(const Rcpp::NumericMatrix);
	void setK(const double);
	void setCentroids(const Rcpp::List);
	void setNrOfClusters(const int);
	void setNstart(const int);
	void setMethodType(const std::string);
	void setMethodInit(const std::string);
	void setR(const double);
	void setCov(const Rcpp::NumericMatrix);
	void setMix(const Rcpp::List);
	void setFunction(const std::string);
	void setEps();
	void setItmax();
	void setLogEnergy(bool);
	void setNCluster(bool);
	void setIters();
};

} /* namespace gmum */

#endif /* CECCONFIGURATION_HPP_ */
