#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "svm_basic.h"
#include "norm.h"
#include <vector>

class SVMClient {
private:
	std::vector<SVMHandler*> SVMHandlers;
	SVMConfiguration config;
	void createFlow();
public:
	SVMConfiguration getConfig();
	arma::vec getResult();
	SVMClient(SVMConfiguration*);
	void run();
};

#endif
