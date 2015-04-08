#include <armadillo>
#include <iostream>
#include <string>
#include "gtest/gtest.h"

#include "svm_helpers.h"
#include "svm/log.h"
#include "svm_basic.h"

namespace {

int log_level = LogLevel::DEBUG;

double *null_double_ptr = 0;
long *null_long_ptr = 0;
long null_long = 0;

// The fixture for testing class SVMLightRunner
class SVMConfigurationTest: public ::testing::Test {

protected:

    SVMConfigurationTest() {
        std::cout << "Creating SVMConfiguration..." << std::endl << std::flush;
        svm_config = SVMConfiguration();
        std::cout << "Setting logger..." << std::endl << std::flush;
        svm_config.log.verbosity = log_level;

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

        std::cout << "Starting test..." << std::endl << std::flush;
    }

    virtual ~SVMConfigurationTest() {}

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
};


} // end namespace

/* Fixture tests */

TEST_F(SVMConfigurationTest, setSparseData) {
    std::cout << "SVMConfiguration sparse data..." << std::endl;
    svm_config.setSparseData(
        sparse_matrix_csc_01_row_indices,
        sparse_matrix_csc_01_column_pointers,
        sparse_matrix_csc_01_values,
        sparse_matrix_csc_01_nrow,
        sparse_matrix_csc_01_ncol,
        true
    );
    for (size_t i = 0; i < svm_config.sparse_data.n_rows; ++i) {
        for (size_t j = 0; j < svm_config.sparse_data.n_cols; ++ j) {
            ASSERT_EQ(svm_config.sparse_data(i, j), sparse_matrix_csc_01_sp_mat(i, j));
        }
    }
}

