#include <R.h>
#include "test_libsvm.h"
#include "libsvm_runner.h"
using namespace Rcpp;
using namespace arma;

std::string test_model_file_name = "svm_model.p";
std::string test_data = "a1a.txt"; //Matrix instead Armaad
std::string output_filename = "output.txt";

// TestLibsvmm
void test_libsvm(){
    using namespace Rcpp ;

    /*
     * Settings parameters
     */

	SVMConfiguration svm_config;
	svm_config.setModelFilename(test_model_file_name);
	svm_config.setFilename(test_data);
	svm_config.setPrediction(false); // training model
	svm_config.setOutputFilename(output_filename);
	/* SVM TRAINING
	 *
	 */
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);
	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);
//    return x ;
}

void test_libsvm_arma() {
	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr << 1 << 1 << endr;
	vec b;
	b << -1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	svm_config.setModelFilename("bika.txt");
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	svm_config.setFilename("test.txt");
	svm_config.setOutputFilename(output_filename);

	lib_svm_runner.processRequest(svm_config);
}

void test_libsvm_arma_arma() {
	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr << 1 << 1 << endr;
	vec b;
	b << -1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	svm_config.setModelFilename("bika.txt");
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);

	printf("%f", svm_config.result[0]);
	printf("%f", svm_config.result[1]);
}
