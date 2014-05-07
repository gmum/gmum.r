#ifndef SVM_TEST_FLOW_H
#define SVM_TEST_FLOW_H

#include "svm_flow_factory.h"
#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_client.h"

#include <Rcpp.h>
#include <vector>

   
// ============= TEST CLASSES =================

class TestSVMFlowFactory : public SVMFlowFactory {
public :
  TestSVMFlowFactory();
  std::vector<SVMHandler*> createSVMFlow( SVMConfiguration );
};

class TestSVMRunner : public SVMHandler {
  SVMResult processRequest( SVMConfiguration, SVMResult );
	bool canHandle( SVMConfiguration);
};

// Something like that will probably be used as a last block in the chain
SEXP dispatchMessage( SVMResult );

RcppExport SEXP testFlow( SEXP );

#endif