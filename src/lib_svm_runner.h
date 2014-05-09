/*
 * LibSVMRunner.h
 *
 *  Created on: Apr 7, 2014
 *      Author: sacherus
 */
#ifndef LIB_SVM_RUNNER_H
#define LIB_SVM_RUNNER_H

#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include "svm.h"
#include "svm_handler.h"
#include "svm_basic.h"
#include <Rcpp.h>

class LibSVMRunner: public SVM_Handler {
public:
	LibSVMRunner();
	virtual ~LibSVMRunner();
	void processRequest(std::string);
	void processRequest(SVM_Configuration& config, SVM_Result& result,
			svm_parameter&, svm_problem&);
	void svm_predict(SVM_Configuration& config, SVM_Result& result);

	SVM_Result processRequest(SVM_Configuration, SVM_Result);
	bool canHandle(SVM_Configuration);

};

#endif /* LIBSVMRUNNER_H_ */
