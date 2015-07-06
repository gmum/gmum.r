#include "gtest/gtest.h"
#include <armadillo>
#include "libsvm_runner.h"
#include "svm_basic.h"
#include "svm_client.h"
#include "svm_helpers.h"

using namespace arma;

//TEST(LibSVMRunnerTest, TrainingFilePredictionFile) {
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
//TEST(LibSVMRunnerTest, ArmaTrainingPredictionFile) {
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
//TEST(LibSVMRunnerTest, ArmaTrainingArmaPrediction) {
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

TEST(LibSVMRunnerTest, ArmaTrainingConfigModel) {

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

TEST(LibSVMRunnerTest, ArmaTrainingArmaPredictionConfigModel2x2) {
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

TEST(LibSVMRunnerTest, DoubleArmaTrainingDoubleArmaPredictionConfigModel4x2) {

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

TEST(LibSVMRunnerTest, ArmaTrainingArmaPredictionConfigModel4x2) {

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

TEST(LibSVMRunnerTest, ArmaTrainingArmaPredictionConfigModel4x2OtherPoints) {

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

TEST(LibSVMRunnerTest, integration_svmclient_predict) {
	SVMConfiguration svm_config;
    arma::mat A = helper_learning_data_01();
    arma::mat b = helper_learning_target_01();
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

    std::cout << "Testing SVMClient prediction..." << std::endl << std::flush;
	svm_config.data = helper_testing_data_01();
    svm_config.setPrediction(true);
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->predict(svm_config.data);
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < 4; ++i) {
        ASSERT_DOUBLE_EQ(
            client_config.result[i], helper_testing_target_01()[i]);
    }
}

TEST(LibSVMRunnerTest, svm_options_01) {
    std::cout << "Learning..." << std::endl << std::flush;
	SVMConfiguration svm_config;
    arma::mat A = helper_learning_data_01();
    arma::mat b = helper_learning_target_01();
	svm_config.setPrediction(false); // training model
    svm_config.svm_options = "-c 0.24";
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;
	lib_svm_runner.processRequest(svm_config);

    std::cout << "Testing SVMClient prediction..." << std::endl << std::flush;
	svm_config.data = helper_testing_data_01();
    svm_config.setPrediction(true);
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->predict(svm_config.data);
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < 4; ++i) {
        // NOTE: `svm-predict` after `svm-train -c 0.24` predicts 1
        // (incorrectly) for all of this data
        ASSERT_DOUBLE_EQ(client_config.result[i], 1);
    }
}
