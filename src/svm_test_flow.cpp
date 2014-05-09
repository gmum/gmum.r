#include "svm_test_flow.h"

// ============= TEST CLASSES =================

// TestSVMRunner
SVMResult TestSVMRunner::processRequest(SVMConfiguration config,
		SVMResult result) {
	return result;
}
bool TestSVMRunner::canHandle(SVMConfiguration) {
	return true;
}

// TestSVMFlowFactory
TestSVMFlowFactory::TestSVMFlowFactory() {
}
std::vector<SVMHandler*> TestSVMFlowFactory::createSVMFlow(
		SVMConfiguration config) {

	TestSVMRunner runner1;
	TestSVMRunner runner2;
	std::vector<SVMHandler*> handlers;
	handlers.push_back(&runner1);
	handlers.push_back(&runner2);

	return handlers;
}

// Dispatch function
SEXP dispatchMessage(SVMResult result) {
	Rcpp::CharacterVector msg = Rcpp::CharacterVector::create(
			result.getMessage());
	return msg;
}

// TestFlow
SEXP testFlow(SEXP x) {
	std::string msg = Rcpp::as<std::string>(x);
	SVMClient client(msg);
	SVMConfiguration config;
	SVMResult result = client.run(config);
	return dispatchMessage(result);
}

