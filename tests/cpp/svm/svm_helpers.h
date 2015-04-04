#ifndef SVM_HELPERS_H
#define SVM_HELPERS_H

#include <armadillo>

// Useful helpers for multiple test cases

arma::mat helper_learning_data_01();

arma::vec helper_learning_target_01();

arma::vec helper_learning_target_02();

arma::mat helper_testing_data_01();

arma::vec helper_testing_target_01();

arma::vec helper_testing_target_02();

arma::uvec helper_sparse_matrix_csc_01_row_indices();

arma::uvec helper_sparse_matrix_csc_01_column_pointers();

arma::vec helper_sparse_matrix_csc_01_values();

size_t helper_sparse_matrix_csc_01_nrow();

size_t helper_sparse_matrix_csc_01_ncol();

#endif

