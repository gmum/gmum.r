#include "gtest/gtest.h"
#include "utils/cluster_test_reader.hpp"
#include <Rcpp.h>
#include <vector>
TEST(TestReader,4-GaussData){

  ClusterTestReader reader("4-Gauss");
  std::vector<Rcpp::NumericVector> points;
  std::vector<int> tags;
  reader.readData(2,points);
  reader.readAnswer(tags);

  ASSERT_EQ(points.size(),tags.size());
}
