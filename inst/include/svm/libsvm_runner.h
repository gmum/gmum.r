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
	bool save_model_to_config(SVMConfiguration&, svm_parameter*, svm_problem&);
	svm_model* load_model_from_config(SVMConfiguration&, svm_parameter*);
	/*public interface
	 *
	 */
	void processRequest(SVMConfiguration&);
	bool canHandle(SVMConfiguration&);

private:
	svm_node ** armatlib(arma::mat); // arma to libsvm standard
	double *	vectlib(arma::vec); //arma vector to lisvm
	void arma_prediction(SVMConfiguration&);
	svm_parameter* configuration_to_problem(SVMConfiguration&);
	svm_node** SparseToSVMNode(arma::vec& x, size_t r, arma::uvec& rowindex, arma::uvec& colindex);

	struct svm_model *model;
	struct svm_problem prob;
};

#endif /* LIBSVMRUNNER_H_ */
