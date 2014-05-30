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


class LibSVMRunner: public SVMHandler {
public:
	LibSVMRunner();
	virtual ~LibSVMRunner();
	void processRequest(std::string);
	void processRequest(SVMConfiguration&, svm_parameter&, svm_problem&);
	void svm_predict(SVMConfiguration&);

	/*public interface
	 *
	 */
	void processRequest(SVMConfiguration&);
	bool canHandle(SVMConfiguration&);

private:
	svm_node ** armatlib(arma::mat); // arma to libsvm standard
	double *	vectlib(arma::vec); //arma vector to lisvm
};

#endif /* LIBSVMRUNNER_H_ */
