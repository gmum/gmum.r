#include <armadillo>
#include <iostream>
#include <string>
#include "gtest/gtest.h"

#include "svm_helpers.h"
#include "svmlight_runner.h"
#include "svm_basic.h"
#include "svm_client.h"
#include "utils/logger.h"

namespace {

int log_level = LogLevel::DEBUG_LEVEL;

double *null_double_ptr = 0;
long *null_long_ptr = 0;
long null_long = 0;

// The fixture for testing class SVMLightRunner
class SVMLightRunnerTest: public ::testing::Test {

protected:

    SVMLightRunnerTest() {
        std::cout << "Creating SVMLightRunner..." << std::endl << std::flush;
        svmlr = SVMLightRunner();
        std::cout << "Creating second SVMLightRunner..." << std::endl << std::flush;
        second_svmlr = SVMLightRunner();
        std::cout << "Creating SVMConfiguration..." << std::endl << std::flush;
        svm_config = SVMConfiguration();
        std::cout << "Setting logger..." << std::endl << std::flush;
        svm_config.log.verbosity = log_level;
        std::cout << "Creating second SVMConfiguration..." << std::endl << std::flush;
        second_svm_config = SVMConfiguration();
        std::cout << "Setting logger..." << std::endl << std::flush;
        second_svm_config.log.verbosity = log_level;

        learning_data_01 = helper_learning_data_01();
        learning_target_01 = helper_learning_target_01();
        learning_target_02 = helper_learning_target_02();

        testing_data_01 = helper_testing_data_01();
        testing_target_01 = helper_testing_target_01();
        testing_target_02 = helper_testing_target_02();

        sparse_matrix_csc_01_row_indices =
            helper_sparse_matrix_csc_01_row_indices();
        sparse_matrix_csc_01_column_pointers =
            helper_sparse_matrix_csc_01_column_pointers();
        sparse_matrix_csc_01_values =
            helper_sparse_matrix_csc_01_values();
        sparse_matrix_csc_01_nrow =
            helper_sparse_matrix_csc_01_nrow();
        sparse_matrix_csc_01_ncol =
            helper_sparse_matrix_csc_01_ncol();
        sparse_matrix_csc_01_sp_mat =
            helper_sparse_matrix_csc_01_sp_mat();
        sparse_matrix_csc_01_learning_target =
            helper_sparse_matrix_csc_01_learning_target();

        std::cout << "Starting test..." << std::endl << std::flush;
    }

    virtual ~SVMLightRunnerTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    /* Objects declared here can be used by all tests in the test case */

    SVMLightRunner svmlr;
    SVMLightRunner second_svmlr;
    SVMConfiguration svm_config;
    SVMConfiguration second_svm_config;

    arma::mat learning_data_01;
    arma::vec learning_target_01;
    arma::vec learning_target_02;
    arma::mat testing_data_01;
    arma::vec testing_target_01;
    arma::vec testing_target_02;

	arma::uvec sparse_matrix_csc_01_row_indices;
    arma::uvec sparse_matrix_csc_01_column_pointers;
    arma::vec sparse_matrix_csc_01_values;
    size_t sparse_matrix_csc_01_nrow;
    size_t sparse_matrix_csc_01_ncol;
    arma::sp_mat sparse_matrix_csc_01_sp_mat;
    arma::vec sparse_matrix_csc_01_learning_target;
};


} // end namespace

/* Fixture tests */

TEST_F(SVMLightRunnerTest, processRequest_learning) {
    std::cout << "SVMConfiguration data..." << std::endl;
    svm_config.data = learning_data_01;
    std::cout << "SVMConfiguration target..." << std::endl;
    svm_config.target = learning_target_01;
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
    // highest feature index - no assignment to read-only data
    ASSERT_EQ(svm_config.data.n_cols, 4);
    // number of support vectors
    ASSERT_EQ(svm_config.l, 3);
    // threshold b
    ASSERT_DOUBLE_EQ(svm_config.b, 2.8710367416806548e-13);
}

TEST_F(SVMLightRunnerTest, processRequest_classification) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_01;
    svm_config.setPrediction(false);
    svmlr.processRequest(svm_config);

    std::cout << "Testing prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    for (int i = 0; i < 4; ++i) {
        ASSERT_DOUBLE_EQ(svm_config.result[i], testing_target_01[i]);
    }
}

TEST_F(SVMLightRunnerTest, test_globals_cleaning) {
    std::cout << "Testing whether globals are being initialized properly"
        << std::endl << std::flush;
    extern double *primal;
    ASSERT_EQ(primal, null_double_ptr);
    extern double *dual;
    ASSERT_EQ(dual, null_double_ptr);
    extern long precision_violations;
    ASSERT_EQ(precision_violations, null_long);
    extern double *buffer;
    ASSERT_EQ(buffer, null_double_ptr);
    extern long *nonoptimal;
    ASSERT_EQ(nonoptimal, null_long_ptr);
    extern long smallroundcount;
    ASSERT_EQ(smallroundcount, null_long);
    extern long roundnumber;
    ASSERT_EQ(roundnumber, null_long);
    extern long kernel_cache_statistic;
    ASSERT_EQ(kernel_cache_statistic, null_long);
}


TEST_F(SVMLightRunnerTest, processRequest_with_poly_kernel) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.setKernel(std::string("poly"));
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_02;
    svm_config.setPrediction(false);
    svmlr.processRequest(svm_config);

    std::cout << "Testing prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    for (int i = 0; i < 4; ++i) {
        ASSERT_DOUBLE_EQ(svm_config.result[i], testing_target_02[i]);
    }
}

TEST_F(SVMLightRunnerTest, processRequest_with_rbf_kernel) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.setKernel(std::string("rbf"));
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_02;
    svm_config.setPrediction(false);
    svmlr.processRequest(svm_config);

    std::cout << "Testing prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    for (int i = 0; i < 4; ++i) {
        ASSERT_DOUBLE_EQ(svm_config.result[i], testing_target_02[i]);
    }
}

TEST_F(SVMLightRunnerTest, processRequest_with_sigmoid_kernel) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.setKernel(std::string("sigmoid"));
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_02;
    svm_config.setPrediction(false);
    /* C calulated by SVMLight: [avg. x*x]^-1 */
    svm_config.C = 2.8411;
    svmlr.processRequest(svm_config);

    std::cout << "Testing prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    svmlr.processRequest(svm_config);

    for (int i = 0; i < 4; ++i) {
        ASSERT_DOUBLE_EQ(svm_config.result[i], testing_target_02[i]);
    }
}

TEST_F(SVMLightRunnerTest, integration_svmclient_predict) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_01;
    svm_config.setPrediction(false);
    // We must do it that way because we are testing SVMLightRunner here :)
    svmlr.processRequest(svm_config);

    std::cout << "Testing SVMClient prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->predict(testing_data_01);
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < testing_target_01.n_rows; ++i) {
        ASSERT_DOUBLE_EQ(
            client_config.result[i], testing_target_01[i]);
    }
}

TEST_F(SVMLightRunnerTest, integration_svmclient_sparse_predict) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    // Sparse matrix is currently being held as transposed
    svm_config.sparse_data = sparse_matrix_csc_01_sp_mat.t();
    svm_config.target = sparse_matrix_csc_01_learning_target;
    svm_config.setPrediction(false);
    svm_config.setSparse(true);
    svmlr.processRequest(svm_config);

    std::cout << "Testing SVMClient sparse prediction..." << std::endl << std::flush;
    svm_config.setPrediction(true);

    SVMClient *svm_client = new SVMClient(&svm_config);
    ASSERT_EQ(svm_client->isSparse(), true);

    svm_client->sparse_predict(
        sparse_matrix_csc_01_column_pointers,
        sparse_matrix_csc_01_row_indices,
        sparse_matrix_csc_01_values,
        sparse_matrix_csc_01_nrow,
        sparse_matrix_csc_01_ncol
    );
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < sparse_matrix_csc_01_learning_target.n_rows; ++i) {
        ASSERT_DOUBLE_EQ(
            client_config.result[i], sparse_matrix_csc_01_learning_target[i]);
    }
}

TEST_F(SVMLightRunnerTest, svm_options_01) {
    std::cout << "Learning..." << std::endl << std::flush;
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_01;
    svm_config.setPrediction(false);
    svm_config.svm_options = "-c 0.25";
    svmlr.processRequest(svm_config);

    std::cout << "Testing SVMClient prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->predict(testing_data_01);
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < testing_target_01.n_rows; ++i) {
        // NOTE: `svm_predict` after `svm_learn -c 0.25` predicts 1
        // (incorrectly) for all of this data
        ASSERT_DOUBLE_EQ(client_config.result[i], 1);
    }
}

TEST_F(SVMLightRunnerTest, svm_options_02) {
    std::cout << "Learning..." << std::endl << std::flush;
    svm_config.data = learning_data_01;
    svm_config.target = learning_target_01;
    svm_config.setPrediction(false);
    svm_config.svm_options = "-c 1";
    svmlr.processRequest(svm_config);

    std::cout << "Testing SVMClient prediction..." << std::endl << std::flush;
    svm_config.data = testing_data_01;
    svm_config.setPrediction(true);
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->predict(testing_data_01);
    SVMConfiguration client_config = svm_client->getConfiguration();

    for (int i = 0; i < testing_target_01.n_rows; ++i) {
        // NOTE: `svm_predict` after `svm_learn -c 1` predicts with good
        // accuracy for this data
        ASSERT_DOUBLE_EQ(client_config.result[i], testing_target_01[i]);
    }
}
