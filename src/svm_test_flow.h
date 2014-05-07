#ifndef SVM_TEST_FLOW_H
#define SVM_TEST_FLOW_H

#include "svm_flow_factory.h"
#include "svm_handler.h"
#include "svm_basic.h"
#include "svm_client.h"

#include <Rcpp.h>
#include <R.h>
#include <vector>

   
// ============= TEST CLASSES =================

class TestSVMFlowFactory : public SVM_FlowFactory {
public :
  TestSVMFlowFactory();
  std::vector<SVM_Handler*> createSVMFlow( SVM_Configuration );
};

class TestSVMRunner : public SVM_Handler {
  SVM_Result processRequest( SVM_Configuration, SVM_Result );
	bool canHandle( SVM_Configuration);
};

// Something like that will probably be used as a last block in the chain
SEXP dispatchMessage( SVM_Result );

RcppExport SEXP testFlow( SEXP );

#endif
