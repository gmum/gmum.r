#include <string>

#include "svmlight_runner.h"
#include "gtest/gtest.h"

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


} // namespace

/* Fixture tests */

// Memory access test
TEST_F(SVMLightRunnerTest, librarySVMLearn) {
    svmLightRunner.librarySVMLearn();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
