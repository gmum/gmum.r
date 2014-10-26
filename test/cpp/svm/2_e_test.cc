#include "gtest/gtest.h"
#include <armadillo>
#include "libsvm_runner.h"
#include "svm_basic.h"
#include "two_e_svm_post.h"
#include "two_e_svm_pre.h"

using namespace arma;

TEST(TwoESVMTest, TestAll) {
	SVMConfiguration svm_config;
	mat A;
	A << 0 << 0 << endr << 1 << 1 << endr;
	vec b;
	b << -1 << 1;
	svm_config.setPrediction(false); // training model
	svm_config.data = A;
	svm_config.target = b;
	LibSVMRunner lib_svm_runner;

	TwoeSVMPreprocessor two_e_pre_runner;
	TwoeSVMPostprocessor two_e_post_runner;


	two_e_pre_runner.processRequest(svm_config);
	lib_svm_runner.processRequest(svm_config);
	two_e_post_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(1.0, svm_config.result[1]);
}
