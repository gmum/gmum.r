#include <armadillo>
#include <string>

#include "gtest/gtest.h"
#include "svmlight_runner.h"
#include "svm_basic.h"

using namespace arma;

namespace {

// The fixture for testing class SVMLightRunner
class SVMLightRunnerTest: public ::testing::Test {

protected:

    SVMLightRunnerTest() {
        svmLightRunner = SVMLightRunner();
    }

    virtual ~SVMLightRunnerTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case

    SVMLightRunner svmLightRunner;
};


} // end namespace

/* Fixture tests */

TEST_F(SVMLightRunnerTest, processRequest) {
	mat data;
	data << 0.5 << 1.0 << 0.0 << 1.0 << endr
         << 0.4 << 1.1 << 0.0 << 1.0 << endr
         << 0.5 << 0.9 << 1.0 << 0.0 << endr
         << 0.5 << 1.0 << 1.0 << 0.0 << endr
         << 0.4 << 1.1 << 1.0 << 0.0 << endr;
	vec target;
	target << -1 << -1 << 1 << 1 << 1;

	SVMConfiguration svm_config;
	svm_config.setPrediction(false); // training model
	svm_config.data = data;
	svm_config.target = target;

	SVMLightRunner svmlr;
	svmlr.processRequest(svm_config);

	svm_config.setPrediction(true);
	svmlr.processRequest(svm_config);

    // TODO: ASSERT_EQ();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

