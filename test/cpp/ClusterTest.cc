#include "gtest/gtest.h"
#include <RcppArmadillo.h>
#include "src/Cluster.hpp"

//Separate makefile makes it hard to ensure that Cluster.cpp is compiled
//I think we should make global Makefile that compiles everything and also has
//target "test"
//
#include "src/Cluster.cpp"


class ClusterTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    c = Cluster();
  }

  Cluster c;
};

TEST_F(ClusterTest, MeanTest) {
  Rcpp::NumericMatrix dataset(4,5);
  dataset(0,Rcpp::_) = Rcpp::NumericVector::create(1,2,3,4,5);
  dataset(1,Rcpp::_) = Rcpp::NumericVector::create(10,20,30,40,50);
  dataset(2,Rcpp::_) = Rcpp::NumericVector::create(1,2,3,4,5);
  dataset(3,Rcpp::_) = Rcpp::NumericVector::create(1,2,3,4,5);

  std::vector<int> fits(4);
  fits.push_back(1);
  fits.push_back(2);
  fits.push_back(1);
  fits.push_back(1);

  ASSERT_EQ(Rcpp::NumericVector::create(1,2,3,4,5), c.initializeMean(1,fits,dataset));
}
