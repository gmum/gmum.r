#ifndef SVM_TEST_FLOW_H
#define SVM_TEST_FLOW_H

#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_client.h"

#include <Rcpp.h>
#include <R.h>
#include <vector>

// class TestSVMFlowFactory : public SVMFlowFactory {
// public :
//   TestSVMFlowFactory();
//   std::vector<SVMHandler*> createSVMFlow( SVMConfiguration );
// };

class TestSVMRunner : public SVMHandler {
  SVMResult processRequest( SVMConfiguration, SVMResult );
	bool canHandle( SVMConfiguration);
};

class NormRunner : public SVMHandler {
	SVMResult processRequest( SVMConfiguration, SVMResult );
	bool canHandle ( SVMConfiguration );
};

// Something like that will probably be used as a last block in the chain
SEXP dispatchMessage( SVMResult );

RcppExport SEXP testFlow( SEXP );
RcppExport SEXP dispatchDouble ( SVMResult );

#endif
