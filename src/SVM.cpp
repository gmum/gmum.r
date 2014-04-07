#include "SVM.h"

// SVM Configuration 
  // Constructors  
SVMConfiguration::SVMConfiguration(){}
SVMConfiguration::SVMConfiguration( SVMData *data, SVMParameters params ) {
	this->data = data;
  this->params = params;
}
  // Getters and Setters
void SVMConfiguration::setData( SVMData *data ) {
  this->data = data;
}
SVMData* SVMConfiguration::getData() {
  return this->data;
}
void SVMConfiguration::setParams( SVMParameters params ) {
  this->params = params;
}
SVMParameters SVMConfiguration::getParams() {
  return this->params;
}

// SVM Result
  // Constructors
SVMResult::SVMResult(){}
SVMResult::SVMResult( std::string message ) {
  this->message = message;
}
SVMResult::SVMResult( SVMData *data ) {
	this->data = data;
}
  // Getters and Setters
void SVMResult::setResult( SVMData *data ) {
  this->data = data;
}
SVMData* SVMResult::getResult() {
  return this->data;
}
void SVMResult::setMessage( std::string message ) {
  this->message = message;
}
std::string SVMResult::getMessage() {
  return this->message;
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

// SVMFlowFactory | At the moments returns empty vector<hanlder>
std::vector<SVMHandler*> SVMFlowFactory::createSVMFlow( SVMConfiguration config ) {
  std::vector<SVMHandler*> handlers;
  return handlers;
  // TODO
}

// SVMClient
SVMClient::SVMClient() {
  flowFactory = SVMFlowFactory();
}
SVMClient::SVMClient( std::string message ) {
  flowFactory = SVMFlowFactory();
  this->message = message;
}

// Main client function, it uses FlowFactory to get a certain work flow
// and runs processRequest(c,r) function on each block returing last result
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
  Rcpp::CharacterVector msg = Rcpp::CharacterVector::create( result.getMessage() )  ;
  return msg ;
}

// TestFlow
SEXP testFlow( SEXP x ) {
  std::string msg = Rcpp::as<std::string>(x);
  SVMClient client( msg );
  SVMConfiguration config;
  SVMResult result = client.run(config);
  return dispatchMessage( result );
}


