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
	void processRequest(SVMConfiguration& config, SVMResult& result,
			svm_parameter&, svm_problem&);
	void svm_predict(SVMConfiguration& config, SVMResult& result);

	/*public interface
	 *
	 */
	SVMResult processRequest(SVMConfiguration, SVMResult);
	bool canHandle(SVMConfiguration);

private:
	void armaToLibSVM(SVMData &data);

};

#endif /* LIBSVMRUNNER_H_ */
