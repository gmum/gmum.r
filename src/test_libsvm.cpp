#include "test_libsvm.h"
#include "svm_basic.h"
#include "lib_svm_runner.h"

SEXP rcpp_hello_world(){
    using namespace Rcpp ;
    
    CharacterVector x = CharacterVector::create( "kup", "bar" )  ;
    NumericVector y   = NumericVector::create( 3.0, 1.0 ) ;
    List z            = List::create( x, y ) ;
    
	char model_file_name[] = "svm_model.p";
	std::string test = "a1a.txt";
	SVM_Configuration svm_config;
	SVM_Result svm_result;
	svm_config.setFilename(test);

	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config, svm_result);
	lib_svm_runner.svm_predict("a1a.txt", model_file_name, "output.txt");

    return z ;
}
