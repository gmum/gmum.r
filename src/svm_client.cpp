#include "svm_client.h"
#include <vector>

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

void SVMClient::setMessage( std::string ) {
	this->message = message;
}

std::string SVMClient::getMessage() {
	return this->message;
}