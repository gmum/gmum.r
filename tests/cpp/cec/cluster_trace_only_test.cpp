#include "gtest/gtest.h"
#include "cluster.hpp"
#include <armadillo>
#include <boost/shared_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace gmum;

TEST(TraceOnly,AddPoint) {
    //arma_rng::set_seed(0);
    int n = 10000;
    int dim = 2;
    int beg = dim+1;
    double acceptable_difference = 1e-6;

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

    ASSERT_TRUE(true);

    boost::shared_ptr<Cluster> m(new ClusterSpherical(id,fits,init_matrix));
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
        ClusterOnlyTrace * upref = dynamic_cast<ClusterOnlyTrace*>(m.get());
        ClusterStandard tmp(id,fits,tmp_matrix);
        arma::rowvec mean_online_difference = upref->get_mean() - real_m;
        float trace_diff = upref->get_cov_mat_trace() - arma::trace(covariance);
        // float relative_error = std::abs(trace_diff/arma::trace(covariance));

        EXPECT_EQ(m->size(),tmp.size());
        EXPECT_LT(std::abs(trace_diff),acceptable_difference);
        // std::cout << i << " : " << relative_error << std::endl;
        for (int j = 0; j < dim; ++j) {
            EXPECT_LT(std::abs(mean_online_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
        }
    }
}

TEST(TraceOnly,removePoint) {
    //arma_rng::set_seed(0);
    int n = 100;
    int dim = 2;
    int end = dim+1;
    double acceptable_difference = 1e-6;

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

    boost::shared_ptr<Cluster> m(new ClusterSpherical(id,fits,init_matrix));
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

        ClusterOnlyTrace * upref = dynamic_cast<ClusterOnlyTrace*>(m.get());
        arma::rowvec mean_online_difference = upref->get_mean() - real_m;
        float trace_diff = upref->get_cov_mat_trace() - arma::trace(covariance);
        // float relative_error = std::abs(trace_diff/arma::trace(covariance));
        // std::cout << i << " " << relative_error << std::endl;
        EXPECT_EQ(m->size(),tmp.size());
        EXPECT_LT(std::abs(trace_diff), acceptable_difference);
        for (int j = 0; j < dim; ++j) {
            EXPECT_LT(std::abs(mean_online_difference(j)),acceptable_difference) << "at position" << j << " means differ by more than " << acceptable_difference;
        }
    }
    ASSERT_TRUE(true);
}
