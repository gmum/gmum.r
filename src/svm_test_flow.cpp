
#include "svm_test_flow.h"

// ============= TEST CLASSES =================

// TestSVMRunner
SVM_Result TestSVMRunner::processRequest( SVM_Configuration config, SVM_Result result) {
  return result;
}
bool TestSVMRunner::canHandle( SVM_Configuration) {
  return true;
}

// TestSVMFlowFactory
TestSVMFlowFactory::TestSVMFlowFactory(){}
std::vector<SVM_Handler*> TestSVMFlowFactory::createSVMFlow( SVM_Configuration config) {  
  
  TestSVMRunner runner1;
  TestSVMRunner runner2;
  std::vector<SVM_Handler*> handlers;
  handlers.push_back( &runner1 );
  handlers.push_back( &runner2 );
  
  return handlers;
}

// Dispatch function
SEXP dispatchMessage( SVM_Result result ) {
  Rcpp::CharacterVector msg = Rcpp::CharacterVector::create( result.getMessage() )  ;
  return msg ;
}

// TestFlow
SEXP testFlow( SEXP x ) {
  std::string msg = Rcpp::as<std::string>(x);
  SVM_Client client( msg );
  SVM_Configuration config;
  SVM_Result result = client.run(config);
  return dispatchMessage( result );
}


