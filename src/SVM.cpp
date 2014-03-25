#include "SVM.h"

// SVM Configuration 
SVMConfiguration::SVMConfiguration(){}

SVMConfiguration::SVMConfiguration( std::map<char*,char*> *map, SVMParameters *params ) {
	this->data = map;
  this->params = params;
}

// SVM Result
SVMResult::SVMResult(){}

SVMResult::SVMResult( const char *message ) {
  this->message = message;
}

SVMResult::SVMResult( std::map<char*,char*> *map ) {
	this->result = map;
}

// LIBSVM Runner NOTE: Empty!
SVMResult LIBSVMRunner::processRequest( SVMConfiguration data, SVMResult result ) { 
  // TODO
  return result;
}

bool LIBSVMRunner::canHandle( SVMConfiguration data ) {
  // TODO
	return true;
}

// SVMFlowFactory
std::vector<SVMHandler*> SVMFlowFactory::createSVMFlow( SVMConfiguration config ) {
  std::vector<SVMHandler*> handlers;
  return handlers;
}

// SVMClient
SVMClient::SVMClient() {
  flowFactory = TestSVMFlowFactory();
}

SVMClient::SVMClient( const char *message ) {
  flowFactory = TestSVMFlowFactory(); // Test Factory
  this->message = message;
}

SVMResult SVMClient::run( SVMConfiguration config) {
  SVMResult result(this->message);
  
  SVMHandlers = flowFactory.createSVMFlow( config );
  
  for ( std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin() ; iter != SVMHandlers.end() ; ++iter ) {
    result = (*iter)->processRequest( config, result );
  }
  return result;
}

// ============= TEST CLASSES =================

// TestSVMRunner
SVMResult TestSVMRunner::processRequest( SVMConfiguration config, SVMResult result) {
  return result;
}
bool TestSVMRunner::canHandle( SVMConfiguration) {
  return true;
}

// TestSVMFlowFactory
TestSVMFlowFactory::TestSVMFlowFactory(){}
std::vector<SVMHandler*> TestSVMFlowFactory::createSVMFlow( SVMConfiguration config) {  
  
  TestSVMRunner runner1;
  TestSVMRunner runner2;
  std::vector<SVMHandler*> handlers;
  handlers.push_back( &runner1 );
  handlers.push_back( &runner2 );
  
  return handlers;
}

// Dispatch function
SEXP dispatchMessage( SVMResult result ) {
  Rcpp::CharacterVector msg = Rcpp::CharacterVector::create( result.message )  ;
  return msg ;
}

// TestFlow
SEXP testFlow( SEXP x ) {
  std::string msg = Rcpp::as<std::string>(x);
  SVMClient client( msg.c_str() );
  SVMConfiguration config;
  SVMResult result = client.run(config);
  return dispatchMessage( result );
}


