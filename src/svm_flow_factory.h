#ifndef SVM_FLOW_FACTORY_H
#define SVM_FLOW_FACTORY_H

// #include "libsvm_runner.h"
#include "norm.h"
#include <vector>

// Flow Factory
class SVMFlowFactory {
public:
	std::vector<SVMHandler*> createSVMFlow(SVMConfiguration*);
};

#endif
