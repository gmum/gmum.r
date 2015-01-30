
#include <iostream>
#include <armadillo>

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

