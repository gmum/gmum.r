#include "svm_client.h"

// SVMClient
SVMClient::SVMClient() {
	flowFactory = SVMFlowFactory();
}
SVMClient::SVMClient(std::string message) {
	flowFactory = SVMFlowFactory();
	this->message = message;
}

// Main client function, it uses FlowFactory to get a certain work flow
// and runs processRequest(c,r) function on each block returing last result
void SVMClient::run(SVMConfiguration *config) {
	SVMHandlers = flowFactory.createSVMFlow(config);
	for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
			iter != SVMHandlers.end(); ++iter) {
		(*iter)->processRequest(*config);
	}
}

void SVMClient::setMessage(std::string) {
	this->message = message;
}

std::string SVMClient::getMessage() {
	return this->message;
}
