#ifndef SVM_FLOW_FACTORY_H
#define SVM_FLOW_FACTORY_H

#include "svm_basic.h"
#include "svm_handler.h"
#include <vector>

// Flow Factory
class SVM_FlowFactory {
public :
  std::vector<SVM_Handler*> createSVMFlow( SVM_Configuration );    
};

#endif