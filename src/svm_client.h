#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "svm_flow_factory.h"
#include "svm_basic.h"
#include <vector>

class SVMClient {
private:
	SVMFlowFactory flowFactory;
	std::vector<SVMHandler*> SVMHandlers;
	std::string message;
public:
	SVMClient();
	SVMClient(std::string);
	void run(SVMConfiguration*);

	void setMessage(std::string);
	std::string getMessage();
};

#endif
