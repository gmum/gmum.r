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

struct SVMData {
	// TODO: Data format
};
 
// Our "input" class containing SVM paramaters and data to be classified
class SVMConfiguration  {
private :
	SVMData *data; 
  	SVMParameters params;
public :	
	SVMConfiguration ();
	SVMConfiguration ( SVMData*, SVMParameters ); 

	void setData( SVMData*);
	SVMData* getData();

	void setParams( SVMParameters );
	SVMParameters getParams();
}; 
  
// Our "output" class containing classification result
class SVMResult {
private :
	SVMData *data; 
  	std::string message;
public : 	
	SVMResult();
  	SVMResult( std::string );
	SVMResult( SVMData* ); 
	SVMResult( SVMData*, std::string );

	void setResult( SVMData* );
	SVMData* getResult();

	void setMessage( std::string );
	std::string getMessage();
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
  std::string message;
public :
  SVMClient();
  SVMClient( std::string );
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

// Something like that will probably be used as a last block in the chain
SEXP dispatchMessage( SVMResult );

RcppExport SEXP testFlow( SEXP );

#endif