#include <armadillo>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "svmlight_runner.h"
#include "svm_basic.h"

namespace {

// The fixture for testing class SVMLightRunner
class SVMLightRunnerTest: public ::testing::Test {

protected:

    SVMLightRunnerTest() {
        svmLightRunner = SVMLightRunner();

    	learing_data_01
            << 0.5 << 1.0 << 0.0 << 1.0 << arma::endr
            << 0.4 << 1.1 << 0.0 << 1.0 << arma::endr
            << 0.5 << 0.9 << 1.0 << 0.0 << arma::endr
            << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr
            << 0.4 << 1.1 << 1.0 << 0.0 << arma::endr;
    	learing_target_01 << -1 << -1 << 1 << 1 << 1;

    	testing_data_01
            << 0.4 << 0.9 << 0.0 << 1.0 << arma::endr
            << 0.5 << 0.9 << 0.0 << 1.0 << arma::endr
            << 0.4 << 1.0 << 1.0 << 0.0 << arma::endr
            << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr;
    	testing_target_01 << -1 << -1 << 1 << 1;
    }

    virtual ~SVMLightRunnerTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    /* Objects declared here can be used by all tests in the test case */

    SVMLightRunner svmLightRunner;

	arma::mat learing_data_01;
    arma::vec learing_target_01;
	arma::mat testing_data_01;
    arma::vec testing_target_01;
};


} // end namespace

/* Fixture tests */

TEST_F(SVMLightRunnerTest, processRequest_learning) {
	SVMLightRunner svmlr;
	SVMConfiguration svm_config;

	svm_config.data = learing_data_01;
	svm_config.target = learing_target_01;
	svm_config.setPrediction(false); // training model
	svmlr.processRequest(svm_config);
    std::cout << "support_vectors:" << svm_config.support_vectors << std::endl;

	svm_config.data = testing_data_01;
	svm_config.target = testing_target_01;
	svm_config.setPrediction(true);
	svmlr.processRequest(svm_config);

    // TODO: ASSERT_EQ();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

