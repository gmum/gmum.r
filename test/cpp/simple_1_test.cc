#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
#include "src/CEC.hpp"
#include <vector>
#include <armadillo>

TEST(Simple_1,IsEnergyCorrect) {
  std::vector<unsigned int > clustering(9,0);
  ClusterReader clusterReader("simple_1",2);
  //clusterReader.getClustering(clustering);
  
  arma::mat points= clusterReader.getPointsInMatrix();
   double killThreshold = 0.0001;
  CEC * cec;
  Hartigan hartigan;
  
  cec = new CEC(points,clustering, killThreshold,  hartigan, 1);
  std::cout << cec->entropy() << std::endl;
  std::cout << clusterReader.getEnergy() << std::endl;
  EXPECT_LT(std::abs(cec->entropy() - clusterReader.getEnergy()) , 1e-4);
  

}
