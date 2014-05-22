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
#include "libsvm/svm.h"
#include "svm_handler.h"
#include "svm_basic.h"


class LibSVMRunner: public SVMHandler {
public:
	LibSVMRunner();
	virtual ~LibSVMRunner();
	void processRequest(std::string);
	void processRequest(SVMConfiguration& config, svm_parameter&, svm_problem&);
	void svm_predict(SVMConfiguration& config);

	/*public interface
	 *
	 */
	void processRequest(SVMConfiguration&);
	bool canHandle(SVMConfiguration&);

private:
	void armaToLibSVM(SVMConfiguration &data);

};

#endif /* LIBSVMRUNNER_H_ */
