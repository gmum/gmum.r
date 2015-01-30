#include "gtest/gtest.h"
#include <RcppArmadillo.h>
#include "libsvm_runner.h"
#include "svm_basic.h"
#include "two_e_svm_post.h"
#include "two_e_svm_pre.h"
#include "svm_utils.h"

using namespace arma;

TEST(SVMUtilsT7est, SqrtInvOfMatrix) {
	double epsilon = 0.05;
	mat A;
	A << 3 << 4 << 4 << endr
			<< 1 << 3 << 4 << endr
			<< 5 << 3 << 2 <<endr
			<< 4 << 7 << 4 << endr
			<< 8 << 9 << 4;
	A = cov(A);
	arma::mat sqrtInv;
	SvmUtils::sqrtInvMat(A, sqrtInv);
	ASSERT_LT(abs(arma::norm(sqrtInv * sqrtInv * A) - 1), epsilon);
}

TEST(TwoESVMTest, TestPreprocessor) {
	SVMConfiguration svm_config;
	double epsilon = 0.05;
	mat posMat, negMat, SqrtInv;
	posMat << 1.3 << 1.5 << endr << 2.0 << 2.3 << endr << 3.0 << 4.0;
	negMat << 0.2 << 0.4 << endr << 0.1 << 0.3 << endr << 0.2 << 0.5;

	vec TrainingTarget;
	TrainingTarget << 1 << 1 << 1 << -1 << -1 << -1;

	svm_config.setPrediction(false); // training model
	svm_config.data = arma::join_vert(posMat, negMat);
	svm_config.target = TrainingTarget;

	TwoeSVMPreprocessor two_e_pre_runner;
	two_e_pre_runner.processRequest(svm_config);
	arma::mat my_cov = cov(posMat) + cov(negMat);
	SvmUtils::sqrtInvMat(my_cov, SqrtInv);
	ASSERT_LT(abs(arma::norm(two_e_pre_runner.cov0InvSqrt - SqrtInv)), epsilon);
}


TEST(TwoESVMTest, TestAll) {
	SVMConfiguration svm_config;
	mat TrainingMatrix;
	TrainingMatrix << 0.2 << 0.4 << endr << 0.1 << 0.3 << endr << 0.2 << 0.5
			<< endr << 1.3 << 1.5 << endr << 2.0 << 2.3 << endr << 3.0 << 4.0;

	vec TrainingTarget;
	TrainingTarget << -1 << -1 << -1 << 1 << 1 << 1;

	mat A;
	A << -1 << -1 << endr << -0.5 << -0.5 << endr << 2 << 1.5 << endr << 3 << 3;

	svm_config.setPrediction(false); // training model
	svm_config.data = TrainingMatrix;
	svm_config.target = TrainingTarget;
	LibSVMRunner lib_svm_runner;

	TwoeSVMPreprocessor two_e_pre_runner;
	TwoeSVMPostprocessor two_e_post_runner;

	two_e_pre_runner.processRequest(svm_config);
	lib_svm_runner.processRequest(svm_config);
	two_e_post_runner.processRequest(svm_config);

	svm_config.setPrediction(true);
	svm_config.data = A;
	two_e_pre_runner.processRequest(svm_config);
	lib_svm_runner.processRequest(svm_config);

	ASSERT_EQ(-1.0, svm_config.result[0]);
	ASSERT_EQ(-1.0, svm_config.result[1]);
	ASSERT_EQ(1.0, svm_config.result[2]);
	ASSERT_EQ(1.0, svm_config.result[3]);
}

