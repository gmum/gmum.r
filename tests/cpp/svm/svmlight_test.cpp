#include <armadillo>
#include <iostream>
#include <string>
#include "gtest/gtest.h"

#include "svm/log.h"
#include "svmlight_runner.h"
#include "svm_basic.h"

namespace {

// The fixture for testing class SVMLightRunner
class SVMLightRunnerTest: public ::testing::Test {

protected:

    SVMLightRunnerTest() {
        svmlr = SVMLightRunner();
        svm_config = SVMConfiguration();
        svm_config.log.verbosity = LogLevel::TRACE;

        learing_data_01
            << 0.5 << 1.0 << 0.0 << 1.0 << arma::endr
            << 0.4 << 1.1 << 0.0 << 1.0 << arma::endr
            << 0.5 << 0.9 << 1.0 << 0.0 << arma::endr
            << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr
            << 0.4 << 1.1 << 1.0 << 0.0 << arma::endr;
        learing_target_01 << -1 << -1 << 1 << 1 << 1;

        learing_target_02 << 2 << 2 << 4 << 4 << 4;

        testing_data_01
            << 0.4 << 0.9 << 0.0 << 1.0 << arma::endr
            << 0.5 << 0.9 << 0.0 << 1.0 << arma::endr
            << 0.4 << 1.0 << 1.0 << 0.0 << arma::endr
            << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr;
        testing_target_01 << -1 << -1 << 1 << 1;

        testing_target_02 << 2 << 2 << 4 << 4;
    }

    virtual ~SVMLightRunnerTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    /* Objects declared here can be used by all tests in the test case */

    SVMLightRunner svmlr;
    SVMConfiguration svm_config;

    arma::mat learing_data_01;
    arma::vec learing_target_01;
    arma::vec learing_target_02;
    arma::mat testing_data_01;
    arma::vec testing_target_01;
    arma::vec testing_target_02;
};


} // end namespace

/* Fixture tests */

TEST_F(SVMLightRunnerTest, processRequest_learning) {
    std::cout << "SVMConfiguration data..." << std::endl;
    svm_config.data = learing_data_01;
    std::cout << "SVMConfiguration target..." << std::endl;
    svm_config.target = learing_target_01;
    std::cout << "SVMConfiguration setPrediction..." << std::endl;
    svm_config.setPrediction(false);
    std::cout << "Processing request." << std::endl;
    svmlr.processRequest(svm_config);

    // kernel_type - LINEAR
    ASSERT_EQ(svm_config.kernel_type, 0);
    // -d int      -> parameter d in polynomial kernel
    ASSERT_EQ(svm_config.degree, 3);
    // -g float    -> parameter gamma in rbf kernel
    ASSERT_EQ(svm_config.gamma, 1);
    // -s float    -> parameter s in sigmoid/poly kernel
    ASSERT_EQ(svm_config.coef0, 1);
    // -r float    -> parameter c in sigmoid/poly kernel
    ASSERT_EQ(svm_config.C, 1);
    // -u string   -> parameter of user defined kernel
    ASSERT_EQ(std::string(svm_config.kernel_parm_custom), std::string("empty"));
    // highest feature index - no assignment to read-only data
    ASSERT_EQ(svm_config.data.n_cols, 4);
    // number of support vectors
    ASSERT_EQ(svm_config.l, 5);
    // threshold b
    ASSERT_DOUBLE_EQ(svm_config.threshold_b, -0.11450359507913976);
}

TEST_F(SVMLightRunnerTest, processRequest_classification) {
    svm_config.data = learing_data_01;
    svm_config.target = learing_target_01;
    svm_config.setPrediction(false);
    svmlr.processRequest(svm_config);

    svm_config.data = testing_data_01;
    svm_config.target = testing_target_01;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    ASSERT_DOUBLE_EQ(svm_config.result[0], -1);
    ASSERT_DOUBLE_EQ(svm_config.result[1], -1);
    ASSERT_DOUBLE_EQ(svm_config.result[2], 1);
    ASSERT_DOUBLE_EQ(svm_config.result[3], 1);
}

TEST_F(SVMLightRunnerTest, processRequest_classification_tagged_classes) {
    svm_config.data = learing_data_01;
    svm_config.target = learing_target_02;
    svm_config.setPrediction(false);
    svmlr.processRequest(svm_config);

    svm_config.data = testing_data_01;
    svm_config.target = testing_target_02;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    std::cout << svm_config.label_negative << std::endl;
    std::cout << svm_config.label_positive << std::endl;

    ASSERT_DOUBLE_EQ(svm_config.result[0], 2);
    ASSERT_DOUBLE_EQ(svm_config.result[1], 2);
    ASSERT_DOUBLE_EQ(svm_config.result[2], 4);
    ASSERT_DOUBLE_EQ(svm_config.result[3], 4);
}

