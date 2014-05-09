#include <R.h>
#include "test_libsvm.h"
#include "libsvm_runner.h"
using namespace Rcpp ;

// TestLibsvmm
void test_libsvm(){
    using namespace Rcpp ;
    //cout << "TEST1" << std::endl;
    //CharacterVector x = CharacterVector::create( "libsvm", "working" ) ;

	std::string test_model_file_name = "svm_model.p";
	std::string test_data = "a1a.txt";
	std::string output_filename = "output.txt";
	SVMConfiguration svm_config;
	svm_config.setModelFilename(test_model_file_name);
	svm_config.setFilename(test_data);
	svm_config.setPrediction(false);
	svm_config.setOutputFilename(output_filename);
	SVMResult SVMResult;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config, SVMResult);
	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config, SVMResult);
//    return x ;
}
