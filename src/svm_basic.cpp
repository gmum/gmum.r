#include "svm_basic.h"

// SVM Configuration 
  // Constructors  
SVM_Configuration::SVM_Configuration(){}
SVM_Configuration::SVM_Configuration( SVM_Data *data, SVM_Parameters params ) {
	this->data = data;
  this->params = params;
}
  // Getters and Setters
void SVM_Configuration::setData( SVM_Data *data ) {
  this->data = data;
}
SVM_Data* SVM_Configuration::getData() {
  return this->data;
}
void SVM_Configuration::setParams( SVM_Parameters params ) {
  this->params = params;
}
SVM_Parameters SVM_Configuration::getParams() {
  return this->params;
}

void SVM_Configuration::setFilename(std::string filename) {
	this->filename = filename;
}
std::string SVM_Configuration::getFilename() {
	return this->filename;
}

void SVM_Configuration::setModelFilename(std::string filename) {
	this->model_filename = filename;
}
std::string SVM_Configuration::getModelFilename() {
	return this->model_filename;
}

void SVM_Configuration::setOutputFilename(std::string filename) {
	this->output_filename = filename;
}
std::string SVM_Configuration::getOutputFilename() {
	return this->output_filename;
}

bool SVM_Configuration::isPrediction() {
	return this->prediction;
}

void SVM_Configuration::setPrediction(bool prediction) {
	this->prediction = prediction;
}




// SVM Result
  // Constructors
SVM_Result::SVM_Result(){}
SVM_Result::SVM_Result( std::string message ) {
  this->message = message;
}
SVM_Result::SVM_Result( SVM_Data *data ) {
	this->data = data;
}
  // Getters and Setters
void SVM_Result::setResult( SVM_Data *data ) {
  this->data = data;
}
SVM_Data* SVM_Result::getResult() {
  return this->data;
}
void SVM_Result::setMessage( std::string message ) {
  this->message = message;
}
std::string SVM_Result::getMessage() {
  return this->message;
}


