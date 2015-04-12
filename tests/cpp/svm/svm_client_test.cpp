#include <armadillo>
#include <iostream>
#include "gtest/gtest.h"

#include "svm_helpers.h"
#include "svm/log.h"
#include "svm_client.h"

namespace {

int log_level = LogLevel::DEBUG;

// The fixture
class SVMClientTest: public ::testing::Test {

protected:

    SVMClientTest() {
        std::cout << "Creating SVMConfiguration..." << std::endl << std::flush;
        svm_config = SVMConfiguration();
        std::cout << "Setting logger..." << std::endl << std::flush;
        svm_config.log.verbosity = log_level;

        // Exemplary sparse matrix
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

    virtual ~SVMClientTest() {}

    /// Called immediately after the constructor (righ before each test)
    virtual void SetUp() {}

    /// Called immediately after each test (right before the destructor)
    virtual void TearDown() {}

    /* Objects declared here can be used by all tests in the test case */

    SVMConfiguration svm_config;

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

TEST_F(SVMClientTest, DISABLED_sparse_data_test) {
    std::cout << "Testing learning..." << std::endl << std::flush;
    svm_config.setSparseData(
        sparse_matrix_csc_01_row_indices,
        sparse_matrix_csc_01_column_pointers,
        sparse_matrix_csc_01_values,
        sparse_matrix_csc_01_nrow,
        sparse_matrix_csc_01_ncol,
        true
    );
    //svm_config.sparse_data = sparse_matrix_csc_01_sp_mat;
    svm_config.target = sparse_matrix_csc_01_learning_target;
    svm_config.setLibrary("libsvm");
    SVMClient *svm_client = new SVMClient(&svm_config);
    svm_client->train();

    std::cout << "Testing prediction..." << std::endl << std::flush;
    svm_client->sparse_predict(
        sparse_matrix_csc_01_row_indices,
        sparse_matrix_csc_01_column_pointers,
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

