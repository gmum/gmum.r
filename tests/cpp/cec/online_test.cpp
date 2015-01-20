#include "gtest/gtest.h"
#include <armadillo>
#include <boost/shared_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "cluster.hpp"
using namespace gmum;

TEST(OnlineFormulas,AddPoint) {
    //arma_rng::set_seed(0);
    int n = 100;
    int dim = 2;
    int beg = dim+1;
    double acceptable_difference = 1e-10;

    std::vector<unsigned int> fits;
    unsigned id = 1;
    arma::mat data(n,dim);
    data.randu();
    for (int i = 0; i < n; ++i) {
        fits.push_back(id);
    }

    arma::mat init_matrix(beg,dim);
    for(int k = 0; k < beg; ++k) {
        for (int j = 0; j < dim; ++j) {
            init_matrix(k,j) = data(k,j);
        }
    }

    Cluster * m(new ClusterStandard(id,fits,init_matrix));
    // Dodajemy element o indeksie i
    for (int i = beg; i < n-1; ++i) {

        arma::mat tmp_matrix(i+1,dim);
        for (int j = 0; j <=i; ++j) {
            tmp_matrix.row(j) = data.row(j);
        }
        arma::mat covariance= cov(tmp_matrix,1);
        arma::mat real_m = mean(tmp_matrix);

        arma::rowvec point(data.row(i));
        m = m->add_point(point);
        ClusterStandard tmp(id,fits,tmp_matrix);
        arma::rowvec mean_online_difference = m->get_mean() - real_m;
        arma::mat mean_init_difference = real_m - tmp.get_mean();
        arma::mat cov_online_difference = m->get_cov_mat(id, fits, tmp_matrix) - covariance;
        arma::mat cov_init_difference = covariance - tmp.get_cov_mat(id, fits, tmp_matrix);

        EXPECT_EQ(m->size(),tmp.size());

        for (int j = 0; j < dim; ++j) {
            EXPECT_LT(std::abs(mean_online_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
            EXPECT_LT(std::abs(mean_init_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
        }

        for (int j = 0; j < dim; ++j) {
            for (int k = 0; k < dim; ++k) {
                EXPECT_LT(std::abs(cov_online_difference(j,k)),acceptable_difference ) << " at position (" << j << "," << k << ")" << "differs by more than " << acceptable_difference;
                EXPECT_LT(std::abs(cov_init_difference(j,k)),acceptable_difference) << " at position (" << j << "," << k << ")" << "differs by more than " << acceptable_difference;
            }
        }
    }
}

TEST(OnlineFormulas,removePoint) {
    //arma_rng::set_seed(0);
    int n = 100;
    int dim = 2;
    int end = dim+1;
    double acceptable_difference = 1e-10;

    std::vector<unsigned int> fits;
    unsigned id = 1;
    arma::mat data(n,dim);
    data.randu();
    for (int i = 0; i < n; ++i) {
        fits.push_back(id);
    }

    arma::mat init_matrix(n,dim);
    for(int k = 0; k < n; ++k) {
        for (int j = 0; j < dim; ++j) {
            init_matrix(k,j) = data(k,j);
        }
    }

    Cluster * m(new ClusterStandard(id,fits,init_matrix));
    // Dodajemy element o indeksie i
    for (int i = n-1; i > end; --i) {

        arma::mat tmp_matrix(i,dim);
        for (int j = 0; j <i; ++j) {
            tmp_matrix.row(j) = data.row(j);
        }
        arma::mat covariance= cov(tmp_matrix,1);
        arma::mat real_m = mean(tmp_matrix);

        arma::rowvec point(data.row(i));
        m = m->remove_point(point);
        ClusterStandard tmp(id,fits,tmp_matrix);
        arma::rowvec mean_online_difference = m->get_mean() - real_m;
        arma::mat mean_init_difference = real_m - tmp.get_mean();
        arma::mat cov_online_difference = m->get_cov_mat(id, fits, tmp_matrix) - covariance;
        arma::mat cov_init_difference = covariance - tmp.get_cov_mat(id, fits, tmp_matrix);

        EXPECT_EQ(m->size(),tmp.size());

        for (int j = 0; j < dim; ++j) {
            EXPECT_LT(std::abs(mean_online_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
            EXPECT_LT(std::abs(mean_init_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
        }

        for (int j = 0; j < dim; ++j) {
            for (int k = 0; k < dim; ++k) {
                EXPECT_LT(std::abs(cov_online_difference(j,k)),acceptable_difference ) << " at position (" << j << "," << k << ")" << "differs by more than " << acceptable_difference;
                EXPECT_LT(std::abs(cov_init_difference(j,k)),acceptable_difference) << " at position (" << j << "," << k << ")" << "differs by more than " << acceptable_difference;
            }
        }
    }
}

