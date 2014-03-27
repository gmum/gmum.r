#include "gtest/gtest.h"
#include <armadillo>
#include "src/Cluster.hpp"

class ClusterTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    c = Cluster();
  }

  Cluster c;
};

TEST_F(ClusterTest, MeanTest) {
  arma::mat dataset(4,5);
  dataset.row(0) = arma::rowvec("1 2 3 4 5");
  dataset.row(1) = arma::rowvec("10 20 30 40 50");
  dataset.row(2) = arma::rowvec("1 2 3 4 5");
  dataset.row(3) = arma::rowvec("1 2 3 4 5");

  std::vector<int> fits(4);
  fits.push_back(1);
  fits.push_back(2);
  fits.push_back(1);
  fits.push_back(1);

  //ASSERT_EQ(arma::rowvec("1 2 3 4 5"), c.initializeMean(1,fits,dataset));
}
