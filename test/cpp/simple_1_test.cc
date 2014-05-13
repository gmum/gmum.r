#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
#include "src/CEC.hpp"
#include <vector>
#include <armadillo>

TEST(Simple_1,IsEnergyCorrect) {
  std::vector<unsigned int> *clustering = new std::vector<unsigned int>();
  ClusterReader clusterReader("simple_1",2);
  clusterReader.getClustering(*clustering);

  int min = *(std::min_element(clustering->begin(),clustering->end()));
  for (std::vector<unsigned int>::iterator it = clustering->begin(); it!= clustering->end() ; ++it)
    *it -= min;

  arma::mat *points = new arma::mat(clusterReader.getPointsInMatrix());
  double killThreshold = 0.0001;
  CEC * cec;
  Hartigan *hartigan = new Hartigan();
  cec = new CEC(points,clustering, killThreshold, hartigan, 1);

  std::cout << cec->entropy() << std::endl;
  std::cout << clusterReader.getEnergy() << std::endl;
  std::cout << " mean : " << cec->clusters[0].getMean() << std::endl;
  std::cout << clusterReader.getEnergy() << std::endl;
  std::cout << " cov : " << cec->clusters[0].getCovMat() << std::endl;
  EXPECT_LT(std::abs(cec->entropy() - clusterReader.getEnergy()) , 1e-4);
}
