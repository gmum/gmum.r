#include "gtest/gtest.h"
#include <armadillo>
#include "libsvm_runner.h"
#include "svm_basic.h"

using namespace arma;

//TEST(LibsvmTest, TrainingFilePredictionFile) {
//	SVMConfiguration svm_config;
//	mat A;
//	A << 0 << 0 << endr << 1 << 1 << endr;
//	vec b;
//	b << -1 << 1;
//	svm_config.setPrediction(false); // training model
//	svm_config.data = A;
//	svm_config.target = b;
//	svm_config.setModelFilename("bika.txt");
//	LibSVMRunner lib_svm_runner;
//	lib_svm_runner.processRequest(svm_config);
//	svm_config.setPrediction(true);
//	svm_config.setFilename("test.txt");
//	svm_config.setOutputFilename("test.txt");
//	lib_svm_runner.processRequest(svm_config);
//}
//
//TEST(LibsvmTest, ArmaTrainingPredictionFile) {
//	using namespace arma;
//	SVMConfiguration svm_config;
//	mat A;
//	A << 0 << 0 << endr << 1 << 1 << endr;
//	vec b;
//	b << -1 << 1;
//	svm_config.setPrediction(false); // training model
//	svm_config.data = A;
//	svm_config.target = b;
//	svm_config.setModelFilename("bika.txt");
//	LibSVMRunner lib_svm_runner;
//	lib_svm_runner.processRequest(svm_config);
//	svm_config.setPrediction(true);
//	svm_config.setFilename("test.txt");
//	svm_config.setOutputFilename("test.txt");
//	lib_svm_runner.processRequest(svm_config);
//}
//
//TEST(LibsvmTest, ArmaTrainingArmaPrediction) {
//
//	SVMConfiguration svm_config;
//	mat A;
//	A << 0 << 0 << endr << 1 << 1 << endr;
//	vec b;
//	b << -1 << 1;
//	svm_config.setPrediction(false); // training model
//	svm_config.data = A;
//	svm_config.target = b;
//	svm_config.setModelFilename("svm_test_model.txt");
//	LibSVMRunner lib_svm_runner;
//	lib_svm_runner.processRequest(svm_config);
//
//	svm_config.setPrediction(true);
//	lib_svm_runner.processRequest(svm_config);
//
//	ASSERT_EQ(-1.0, svm_config.result[0]);
//	ASSERT_EQ(1.0, svm_config.result[1]);
//}

TEST(LibsvmTest, ArmaTrainingConfigModel) {

	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr << 1 << 1 << endr;
	vec b;
	b << -1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

//	svm_config.setPrediction(true);
//	lib_svm_runner.processRequest(svm_config);
//
//	ASSERT_EQ(-1.0, svm_config.result[0]);
//	ASSERT_EQ(1.0, svm_config.result[1]);
}

TEST(LibsvmTest, ArmaTrainingArmaPredictionConfigModel2x2) {
	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr << 1 << 1 << endr;
	vec b;
	b << -1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(1.0, svm_config.result[1]);
}

TEST(LibsvmTest, DoubleArmaTrainingDoubleArmaPredictionConfigModel4x2) {

	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr
	  << 1 << 1 << endr
	  << 2 << 2 << endr
	  << 3 << 3 << endr;
	vec b;
	b << -1 << -1 << 1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);
	lib_svm_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(-1.0, svm_config.result[1]);
	ASSERT_EQ(1.0, svm_config.result[2]);
	ASSERT_EQ(1.0, svm_config.result[3]);
}

TEST(LibsvmTest, ArmaTrainingArmaPredictionConfigModel4x2) {

	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr
	  << 1 << 1 << endr
	  << 2 << 2 << endr
	  << 3 << 3 << endr;
	vec b;
	b << -1 << -1 << 1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(-1.0, svm_config.result[1]);
	ASSERT_EQ(1.0, svm_config.result[2]);
	ASSERT_EQ(1.0, svm_config.result[3]);
}

TEST(LibsvmTest, ArmaTrainingArmaPredictionConfigModel4x2OtherPoints) {

	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr
	  << 1 << 1 << endr
	  << 2 << 2 << endr
	  << 3 << 3 << endr;
	vec b;
	b << -1 << -1 << 1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

	mat A2;
	A2 << 0 << 0 << endr
	  << 30 << 40 << endr
	  << 50 << 60 << endr
	  << -1 << -1 << endr;

	vec b2;
	b2 << -1 << 1 << 1 << -1;

	svm_config.data = A2;
	svm_config.target = b2;

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(1.0, svm_config.result[1]);
	ASSERT_EQ(1.0, svm_config.result[2]);
	ASSERT_EQ(-1.0, svm_config.result[3]);
}


