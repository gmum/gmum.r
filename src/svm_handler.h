#ifndef SVM_HANDLER_H
#define SVM_HANDLER_H

#include "svm_basic.h"

/// Abstract SVM-handling interface -- all blocks will implement this class
class SVMHandler {
public:
    /// Perform tasks based on SVMConfiguration and return SVMResult
	virtual SVMResult processRequest(SVMConfiguration, SVMResult) = 0;
    /// Determine if provided SVMConfiguration can be processed
	virtual bool canHandle(SVMConfiguration) = 0;
};

#endif
