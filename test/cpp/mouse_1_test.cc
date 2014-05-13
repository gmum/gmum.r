#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
#include "src/random_assignment.hpp"
#include "src/CEC.hpp"
#include <vector>
#include <armadillo>

class Mouse1Test : public ::testing::Test {
protected:
  Mouse1Test() {
    clustering = new std::vector<unsigned int>();
    ClusterReader clusterReader("mouse_1",2);
    clusterReader.getClustering(*clustering);
    points = new arma::mat(clusterReader.getPointsInMatrix());
    energy = clusterReader.getEnergy();
    int min = *(std::min_element(clustering->begin(),clustering->end()));
    for(std::vector<unsigned int>::iterator it = clustering->begin();it!=clustering->end() ; ++it) {
      *it -= min;
    }
    numberOfClusters = 3;
    std::cout << "initialized data" << std::endl;
  }
  std::vector<unsigned int> *clustering;
  arma::mat *points;
  double energy;
  int numberOfClusters;
};

TEST_F(Mouse1Test,IsEnergyCorrect) {
  double killThreshold = 0.0001;
  BestPermutationComparator comparator;
  int t = 20;
  int numberOfTimesAcceptable = 0;  
  std::cout << "Should get energy : " << energy;
  for (int i = 0 ; i < t ; ++i) {
    std::vector<unsigned int> *assignment = new std::vector<unsigned int>();
    initAssignRandom(*assignment, points->n_rows, numberOfClusters);
    CEC * cec;
    Hartigan *hartigan = new Hartigan();
    cec = new CEC(points,clustering, killThreshold, hartigan, numberOfClusters);

    cec->loop();
    double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
    std::cout << "Percentage " << percentage << std::endl;
    std::cout << "Energy " << cec->entropy() << std::endl;
    numberOfTimesAcceptable += (percentage >= 0.9) || (cec->entropy() < energy*1.5);
  }  
  EXPECT_GT(numberOfTimesAcceptable , t/2);
}