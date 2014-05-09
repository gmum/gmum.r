#ifndef SVM_FLOW_FACTORY_H
#define SVM_FLOW_FACTORY_H

#include "svm_basic.h"
#include "svm_handler.h"
#include <vector>

// Flow Factory
class SVMFlowFactory {
public:
	std::vector<SVMHandler*> createSVMFlow(SVMConfiguration);
};

#endif
