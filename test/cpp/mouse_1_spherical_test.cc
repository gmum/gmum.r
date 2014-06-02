#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
#include "src/random_assignment.hpp"
#include "src/CEC.hpp"
#include <vector>
#include <armadillo>
#include <boost/smart_ptr.hpp>
using namespace gmum;
class Mouse1SphericalTest : public ::testing::Test {
protected:
  Mouse1SphericalTest() {
    clustering.reset(new std::vector<unsigned int>());
    ClusterReader clusterReader("mouse_1_spherical",2);
    clusterReader.getClustering(*clustering);
    points.reset(new arma::mat(clusterReader.getPointsInMatrix()));
    energy = clusterReader.getEnergy();
    int min = *(std::min_element(clustering->begin(),clustering->end()));
    for(std::vector<unsigned int>::iterator it = clustering->begin();it!=clustering->end() ; ++it) {
      *it -= min;
    }
    numberOfClusters = 3;
    std::cout << "initialized data" << std::endl;
  }
  boost::shared_ptr<std::vector<unsigned int> > clustering;
  boost::shared_ptr<arma::mat> points;
  double energy;
  int numberOfClusters;
};

#define SHOW_CLUSTERING

TEST_F(Mouse1SphericalTest,IsEnergyCorrect) {
  double killThreshold = 0.0001;
  BestPermutationComparator comparator;
  int t = 20;
  int numberOfTimesAcceptable = 0;  
  std::cout << "Should get energy : " << energy;
  for (int i = 0 ; i < t ; ++i) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    initAssignRandom(*assignment, points->n_rows, numberOfClusters);
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
    std::vector<ClusterType> types;
    std::vector<float> radius;
    std::vector<arma::mat> covMatrices;
    for (int i = 0 ; i < numberOfClusters ; ++i){
      types.push_back(spherical);
    }
    CEC cec(points, assignment, hartigan, killThreshold, types,radius,covMatrices);

    cec.loop();
    double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
    std::cout << "Percentage " << percentage << std::endl;
    std::cout << "Energy " << cec.entropy() << std::endl;
    numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
    #ifdef SHOW_CLUSTERING
    std::cout << "BEGIN" << std::endl;
    for (std::vector<unsigned int>::iterator it = assignment->begin() ; it!=assignment->end() ; ++it)
      std::cout << *it << std::endl;
    std::cout << "END" << std::endl;
    #endif

  }  
  EXPECT_GT(numberOfTimesAcceptable , t/2);
}


TEST_F(Mouse1SphericalTest,StartingFromCorrectAssignment) {
  double killThreshold = 0.0001;
  BestPermutationComparator comparator;
  int t = 1;
  int numberOfTimesAcceptable = 0;  
  std::cout << "Should get energy : " << energy;
  for (int i = 0 ; i < t ; ++i) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    for (std::vector<unsigned int>::iterator it = clustering->begin();it!=clustering->end(); ++it)
      assignment->push_back(*it);
    //    initAssignRandom(*assignment, points->n_rows, numberOfClusters);
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
    std::vector<ClusterType> types;
    std::vector<float> radius;
    std::vector<arma::mat> covMatrices;
    for (int i = 0 ; i < numberOfClusters ; ++i){
      types.push_back(spherical);
    }
    CEC cec(points, assignment, hartigan, killThreshold, types,radius,covMatrices);

    cec.loop();
    double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
    std::cout << "Percentage " << percentage << std::endl;
    std::cout << "Energy " << cec.entropy() << std::endl;
    numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
    #ifdef SHOW_CLUSTERING
    std::cout << "BEGIN" << std::endl;
    for (std::vector<unsigned int>::iterator it = assignment->begin() ; it!=assignment->end() ; ++it)
      std::cout << *it << std::endl;
    std::cout << "END" << std::endl;
    #endif

  }  
  EXPECT_GT(numberOfTimesAcceptable , t ==1 ?  0.9 : t/2);
}
// TEST_F(Mouse1SphericalTest,NormalIsEnergyCorrect) {
//     double killThreshold = 0.0001;
//   BestPermutationComparator comparator;
//   int t = 20;
//   int numberOfTimesAcceptable = 0;  
//   std::cout << "Should get energy : " << energy;
//   for (int i = 0 ; i < t ; ++i) {
//     boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
//     initAssignRandom(*assignment, points->n_rows, numberOfClusters);
//     boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
//     CEC cec(points, assignment, hartigan, killThreshold, numberOfClusters);

//     cec.loop();
//     double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
//     std::cout << "Percentage " << percentage << std::endl;
//     std::cout << "Energy " << cec.entropy() << std::endl;
//     numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
//         #ifdef SHOW_CLUSTERING
//     std::cout << "BEGIN" << std::endl;
//     for (std::vector<unsigned int>::iterator it = assignment->begin() ; it!=assignment->end() ; ++it)
//       std::cout << *it << std::endl;
//     std::cout << "END" << std::endl;
//     #endif
//   }  
//   EXPECT_GT(numberOfTimesAcceptable , t/2);
// }
