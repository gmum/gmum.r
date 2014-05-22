
#include "norm.h"

NormRunner::NormRunner(){}
NormRunner::~NormRunner(){}

void NormRunner::processRequest(SVMConfiguration &config) {
//	arma::mat x = config.data;
//	double norm = arma::norm(x);
//	std::vector<double> v;
//	v.push_back(norm);
//	arma::vec target(v);
//	config.result = target;
}

bool NormRunner::canHandle(SVMConfiguration &config) {
	return true;
}

