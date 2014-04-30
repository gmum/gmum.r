/*
 * LibSVMRunner.h
 *
 *  Created on: Apr 7, 2014
 *      Author: sacherus
 */
#ifndef LIBSVMRUNNER_H_
#define LIBSVMRUNNER_H_

#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include "svm.h"
#include "svm_handler.h"
#include "svm_basic.h"

class LibSVMRunner : public SVM_Handler {
public:
	LibSVMRunner();
	virtual ~LibSVMRunner();
	void processRequest(std::string);
	void processRequest(svm_parameter&, svm_problem&);

	SVM_Result processRequest( SVM_Configuration, SVM_Result);
	bool canHandle( SVM_Configuration);

};

#endif /* LIBSVMRUNNER_H_ */
