
#include "svm_helpers.h"

// Useful helpers for multiple test cases

arma::mat helper_learning_data_01() {
    arma::mat matrix;
    matrix
        << 0.5 << 1.0 << 0.0 << 1.0 << arma::endr
        << 0.4 << 1.1 << 0.0 << 1.0 << arma::endr
        << 0.5 << 0.9 << 1.0 << 0.0 << arma::endr
        << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr
        << 0.4 << 1.1 << 1.0 << 0.0 << arma::endr;
    return matrix;
}

arma::vec helper_learning_target_01() {
    arma::vec vector;
    vector
        << -1 << -1 << 1 << 1 << 1;
    return vector;
}

arma::vec helper_learning_target_02() {
    arma::vec vector;
    vector
        << 2 << 2 << 4 << 4 << 4;
    return vector;
}

arma::mat helper_testing_data_01() {
    arma::mat matrix;
    matrix
        << 0.4 << 0.9 << 0.0 << 1.0 << arma::endr
        << 0.5 << 0.9 << 0.0 << 1.0 << arma::endr
        << 0.4 << 1.0 << 1.0 << 0.0 << arma::endr
        << 0.5 << 1.0 << 1.0 << 0.0 << arma::endr;
    return matrix;
}

arma::vec helper_testing_target_01() {
    arma::vec vector;
    vector
        << -1 << -1 << 1 << 1;
    return vector;
}

arma::vec helper_testing_target_02() {
    arma::vec vector;
    vector
        << 2 << 2 << 4 << 4;
    return vector;
}

// Sparse matrix CSC (CCS) data from:
// http://netlib.org/linalg/html_templates/node92.html

arma::uvec helper_sparse_matrix_csc_01_row_indices() {
    arma::uvec vector;
    vector
        << 1 << 2 << 4 << 2 << 3 << 5 << 6 << 3 << 4 << 3 << 4 << 5 << 1 << 4
        << 5 << 6 << 2 << 5 << 6;
    return vector;
}

arma::uvec helper_sparse_matrix_csc_01_column_pointers() {
    arma::uvec vector;
    vector
        << 1 << 4 << 8 << 10 << 13 << 17 << 20;
    return vector;
}

arma::vec helper_sparse_matrix_csc_01_values() {
    arma::vec vector;
    vector
        << 10 << 3 << 3 << 9 << 7 << 8 << 4 << 8 << 8 << 7 << 7 << 9 << -2 << 5
        << 9 << 2 << 3 << 13 << -1;
    return vector;
}

size_t helper_sparse_matrix_csc_01_nrow() {
    return 6;
}

size_t helper_sparse_matrix_csc_01_ncol() {
    return 6;
}

arma::mat helper_sparse_matrix_csc_01_mat() {
    arma::mat matrix;
    matrix
        << 10 << 0 << 0 << 0 << -2 << 0 << arma::endr
        << 3  << 9 << 0 << 0 << 0  << 3 << arma::endr
        << 0  << 7 << 8 << 7 << 0  << 0 << arma::endr
        << 3  << 0 << 8 << 7 << 5  << 0 << arma::endr
        << 0  << 8 << 0 << 9 << 9 << 13 << arma::endr
        << 0  << 4 << 0 << 0 << 2 << -1 << arma::endr;
    return matrix;
}

arma::sp_mat helper_sparse_matrix_csc_01_sp_mat() {
    arma::sp_mat matrix(helper_sparse_matrix_csc_01_mat());
    return matrix;
}

arma::vec helper_sparse_matrix_csc_01_learning_target() {
    arma::vec vector;
    // If there is a 8 in the third column
    vector
        << 1 << 1 << -1 << -1 << 1 << 1;
    return vector;
}

