#ifndef LIBSVM_RUNNER_H
#define LIBSVM_RUNNER_H

#include "svm_handler.h"
#include "svm_basic.h"

// Exemplary SVMRunner for LIBSVM
class LIBSVM_Runner : public SVM_Handler{
public :
	SVM_Result processRequest( SVM_Configuration, SVM_Result );
	bool canHandle( SVM_Configuration);
};

#endif