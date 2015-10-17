#include <string>
#include <iostream>
#include "gtest/gtest.h"

#include "utils/utils.h"

namespace {

/* Fixture */
class UtilsTest: public ::testing::Test {

protected:

    UtilsTest() {}
    virtual ~UtilsTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    /* Objects declared here can be used by all tests in the test case */
    std::string sample_args[5] {
        std::string(""),
        std::string("-a"),
        std::string("-d 4.51"),
        std::string("filename1 filename2 -a 1 -b 0 -c"),
        std::string("filename1 filename2   -a \t 1   -b  0\t\t-c")
    };

    int sample_args_count[5] {
        0,
        1,
        2,
        7,
        7
    };

    char sample_args_argv[5][7][10] = {
        {},
        {"-a"},
        {"-d", "4.51"},
        {"filename1", "filename2", "-a", "1", "-b", "0", "-c"},
        {"filename1", "filename2", "-a", "1", "-b", "0", "-c"}
    };

};

} // end namespace

/* Fixture tests */

TEST_F(UtilsTest, check_argc) {
    int sample_num = sizeof(sample_args)/sizeof(*sample_args);
    for (int i = 0; i < sample_num; ++i) {
        ASSERT_EQ(check_argc(sample_args[i].c_str()), sample_args_count[i]);
        ASSERT_EQ(check_argc(sample_args[i]), sample_args_count[i]);
    }
}

TEST_F(UtilsTest, to_argv) {
    int sample_num = sizeof(sample_args)/sizeof(*sample_args);
    for (int i = 0; i < sample_num; ++i) {
        int argc = check_argc(sample_args[i].c_str());
        char** argv = to_argv(sample_args[i].c_str());

        for(int j = 0; j < argc; ++j) {
            ASSERT_EQ(
                std::string(argv[j]),
                std::string(sample_args_argv[i][j])
            );
        }
    }
}

TEST_F(UtilsTest, free_argv) {
    int sample_num = sizeof(sample_args)/sizeof(*sample_args);
    for (int i = 0; i < sample_num; ++i) {
        int argc = check_argc(sample_args[i].c_str());
        char** argv = to_argv(sample_args[i].c_str());
        argv = free_argv(argc, argv);
        ASSERT_TRUE(argv == NULL);
    }
}
