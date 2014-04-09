#ifndef SVM_CLIENT_H
#define SVM_CLIENT_H

#include "svm_handler.h"
#include "svm_flow_factory.h"
#include "svm_basic.h"
#include <vector>

class SVM_Client {
private :
  SVM_FlowFactory flowFactory;
  std::vector<SVM_Handler*> SVM_Handlers;
  std::string message;
public :
  SVM_Client();
  SVM_Client( std::string );
  SVM_Result run( SVM_Configuration );

  void setMessage( std::string );
  std::string getMessage();
}; 

#endif