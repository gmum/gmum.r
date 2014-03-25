#ifndef _gmum-r_RCPP_HELLO_WORLD_H
#define _gmum-r_RCPP_HELLO_WORLD_H

#include <Rcpp.h>
#include <map>
#include <vector>

// This struct will conatin all the necessery svm parameters and will be used in SVMConfig
struct SVMParameters {
  int cost;
	int degree;
	// TODO : rest of them	
};
 
// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration  {
public :
	std::map<char*, char*> *data; // NOTE: This will be changed when we decide on a input data format
  SVMParameters *params;
		
	SVMConfiguration ();
	SVMConfiguration ( std::map<char*,char*>*, SVMParameters* ); 
}; 
  

// Our "output" class containing classification result
class SVMResult {
public : 
	std::map<char*,char*> *result; // NOTE: This will be changed when we decide on a output data format
  const char *message;
	
	SVMResult();
  SVMResult(const char *message);
	SVMResult( std::map<char*,char*>* ); 
};

// SVMHandler interface, all blocks will implement this class
class SVMHandler {
public :
	virtual SVMResult processRequest( SVMConfiguration, SVMResult ) = 0; 
	virtual bool canHandle( SVMConfiguration ) = 0;
};

// Exemplary SVMRunner for LIBSVM
class LIBSVMRunner : public SVMHandler{
public :
	SVMResult processRequest( SVMConfiguration, SVMResult );
	bool canHandle( SVMConfiguration);
};

// Flow Factory
class SVMFlowFactory {
public :
  std::vector<SVMHandler*> createSVMFlow( SVMConfiguration );    
};

// SVM Client
class SVMClient {
private :
  SVMFlowFactory flowFactory;
  std::vector<SVMHandler*> SVMHandlers;
  const char* message;
public :
  SVMClient();
  SVMClient(const char* );
  SVMResult run( SVMConfiguration );
}; 

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

SEXP dispatchMessage( SVMResult );
RcppExport SEXP testFlow( SEXP );

#endif