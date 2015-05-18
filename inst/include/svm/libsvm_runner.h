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
	svm_node** armatlib(arma::mat); // arma to libsvm standard
	double*	vectlib(arma::vec); //arma vector to lisvm
	void arma_prediction(SVMConfiguration&);
	svm_parameter* configuration_to_problem(SVMConfiguration&);
	svm_node** SparseToSVMNode(
        arma::vec& x, int r, arma::Col<int>& rowindex, arma::Col<int>& colindex
    );
    svm_node** ArmaSpMatToSvmNode(arma::sp_mat &);
    void parseCommandLine(SVMConfiguration&, svm_parameter&);
    /// Parse command line args from `config` and store them in `param`
    void libraryParseCommandLine(
        SVMConfiguration& config,
        svm_parameter& param,
        int argc,
        char** argv,
        char *input_file_name,
        char *model_file_name
    );

	struct svm_model *model;
	struct svm_problem prob;
};

#endif /* LIBSVMRUNNER_H_ */
