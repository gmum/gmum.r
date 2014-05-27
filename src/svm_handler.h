#ifndef SVM_HANDLER_H
#define SVM_HANDLER_H

#include "svm_basic.h"

// SVMHandler interface, all blocks will implement this class
class SVMHandler {
public:
	//virtual ~SVMHandler();
	virtual void processRequest(SVMConfiguration&) = 0;
	virtual bool canHandle(SVMConfiguration&) = 0;
};

#endif
