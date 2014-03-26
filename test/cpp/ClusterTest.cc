#include "gtest/gtest.h"
#include <Rcpp.h>
#include "src/Cluster.hpp"

class ClusterTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    c = Cluster();
  }

  Cluster c;
};

TEST(ClusterTest, MeanTest) {
  Rcpp::NumericMatrix dataset(4,5);
  dataset(0,_) = Rcpp::NumericVector::create(1,2,3,4,5);
  dataset(1,_) = Rcpp::NumericVector::create(10,20,30,40,50);
  dataset(2,_) = Rcpp::NumericVector::create(1,2,3,4,5);
  dataset(3,_) = Rcpp::NumericVector::create(1,2,3,4,5);

  std::vector<int> fits(4);
  fits.push_back(1);
  fits.push_back(2);
  fits.push_back(1);
  fits.push_back(1);

  ASSERT_EQ(Rcpp::NumericVector::create(1,2,3,4,5), c.initializeMean(1,fits,dataset));
}
