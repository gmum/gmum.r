#include <Rcpp.h>
#include <R.h>
#include "test_libsvm.h"
#include "lib_svm_runner.h"
using namespace Rcpp ;

// TestLibsvmm
void test_libsvm(){
    using namespace Rcpp ;
    //cout << "TEST1" << std::endl;
    //CharacterVector x = CharacterVector::create( "libsvm", "working" ) ;

	char model_file_name[] = "svm_model.p";
	std::string test_model_file_name = "svm_model.p";
	std::string test_data = "a1a.txt";
	std::string output_filename = "output.txt";
	SVM_Configuration svm_config;
	svm_config.setModelFilename(test_model_file_name);
	svm_config.setFilename(test_data);
	svm_config.setPrediction(false);
	svm_config.setOutputFilename(output_filename);
	SVM_Result svm_result;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config, svm_result);
	svm_config.setPrediction(true);
	lib_svm_runner.svm_predict(svm_config, svm_result);
//    return x ;
}
