#ifndef LIBSVM_RUNNER_H
#define LIBSVM_RUNNER_H

#include "svm_handler.h"
#include "svm_basic.h"

// Exemplary SVMRunner for LIBSVM
class LIBSVMRunner : public SVMHandler{
public :
	SVMResult processRequest( SVMConfiguration, SVMResult );
	bool canHandle( SVMConfiguration);
};

#endif