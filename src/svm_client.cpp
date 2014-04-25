#include "svm_client.h"
#include <vector>

// SVMClient
SVM_Client::SVM_Client() {
  flowFactory = SVM_FlowFactory();
}
SVM_Client::SVM_Client( std::string message ) {
  flowFactory = SVM_FlowFactory();
  this->message = message;
}

// Main client function, it uses FlowFactory to get a certain work flow
// and runs processRequest(c,r) function on each block returing last result
SVM_Result SVM_Client::run( SVM_Configuration config) {
  SVM_Result result(this->message);  
  
  SVM_Handlers = flowFactory.createSVMFlow( config );
  
  for ( std::vector<SVM_Handler*>::iterator iter = SVM_Handlers.begin() ; iter != SVM_Handlers.end() ; ++iter ) {
    result = (*iter)->processRequest( config, result );
  }
  return result;
}

void SVM_Client::setMessage( std::string ) {
	this->message = message;
}

std::string SVM_Client::getMessage() {
	return this->message;
}