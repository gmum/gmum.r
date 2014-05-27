/*
 * norm.h
 *
 *  Created on: May 16, 2014
 *      Author: igor
 */

#ifndef NORM_H
#define NORM_H

#include "svm_basic.h"
#include "svm_handler.h"
#include <RcppArmadillo.h>

class NormRunner: public SVMHandler {

public:
	NormRunner();
	virtual ~NormRunner();
	void processRequest(SVMConfiguration &config);
	bool canHandle(SVMConfiguration &config);
};

#endif
