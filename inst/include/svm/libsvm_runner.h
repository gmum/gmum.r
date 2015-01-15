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
	bool SaveModelToConfig(SVMConfiguration&, svm_parameter*, svm_problem&);
	svm_model* LoadModelFromConfig(SVMConfiguration&, svm_parameter*);
	/*public interface
	 *
	 */
	void ProcessRequest(SVMConfiguration&);
	bool CanHandle(SVMConfiguration&);

private:
	svm_node ** Armatlib(arma::mat); // arma to libsvm standard
	double *	Vectlib(arma::vec); //arma vector to lisvm
	void ArmaPrediction(SVMConfiguration&);
	svm_parameter* ConfigurationToProblem(SVMConfiguration&);

	struct svm_model *model_;
	struct svm_problem prob_;
};

#endif /* LIBSVMRUNNER_H_ */
