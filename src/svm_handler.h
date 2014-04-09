#ifndef SVM_HANDLER_H
#define SVM_HANDLER_H

#include "svm_basic.h"

// SVMHandler interface, all blocks will implement this class
class SVM_Handler {
public :
	virtual SVM_Result processRequest( SVM_Configuration, SVM_Result ) = 0; 
	virtual bool canHandle( SVM_Configuration ) = 0;
};

#endif