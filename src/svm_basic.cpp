#include "svm_basic.h"

// SVM Configuration 
  // Constructors  
SVMConfiguration::SVMConfiguration(){}
SVMConfiguration::SVMConfiguration( SVMData *data, SVMParameters params ) {
	this->data = data;
  this->params = params;
}
  // Getters and Setters
void SVMConfiguration::setData( SEXP x, SEXP y ) {
  Rcpp::NumericMatrix xr(x); // Rccp matrix from R data
  Rcpp::NumericVector yr(y); // Rcpp vector from R data
  
  arma::mat X( xr.begin(), xr.nrow(), xr.ncol(), false ); // reusing memory
  arma::vec Y( yr.begin(), yr.size(), false ); 

  SVMData data_struct;
  data_struct.data = X;
  data_struct.target = Y;
  data_struct.len = Y.n_elem;

  this->data = &data_struct;
}
SVMData* SVMConfiguration::getData() {
  return this->data;
}
void SVMConfiguration::setParams( SVMParameters params ) {
  this->params = params;
}
SVMParameters SVMConfiguration::getParams() {
  return this->params;
}

// SVM Result
  // Constructors
SVMResult::SVMResult(){}
SVMResult::SVMResult( std::string message ) {
  this->message = message;
}
SVMResult::SVMResult( SVMData *data ) {
	this->data = data;
}
  // Getters and Setters
void SVMResult::setResult( SVMData *data ) {
  this->data = data;
}
SVMData* SVMResult::getResult() {
  return this->data;
}
void SVMResult::setMessage( std::string message ) {
  this->message = message;
}
std::string SVMResult::getMessage() {
  return this->message;
}