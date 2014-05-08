#include "svm_flow_factory.h"
#include "svm_test_flow.h"
#include <vector>

std::vector<SVMHandler*> SVMFlowFactory::createSVMFlow( SVMConfiguration config ) {

	std::string test = config.getTest();

	if ( test == "norm" ) {
		std::vector<SVMHandler*> handlers;
		NormRunner runner;
		handlers.push_back( &runner );
		return handlers;
	}
	else {
		std::vector<SVMHandler*> handlers;
		return handlers;	
	}  
}