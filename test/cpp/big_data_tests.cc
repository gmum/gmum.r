#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "src/Hartigan.hpp"
#include "src/CEC.hpp"
#include <vector>
#include <armadillo>
#include <boost/smart_ptr.hpp>
#include "src/random_assignment.hpp"
using namespace gmum;

#define SHOW(x) std::cout << #x << " = " << x << std::endl
int times = 10;
void run(const char * str, int numberOfClusters, int times = 1) {
  std::cout.precision(41);
  boost::shared_ptr<std::vector<unsigned int> > clustering(new std::vector<unsigned int>());
  ClusterReader clusterReader(str,4);
  clusterReader.getClustering(*clustering);

  int min = *(std::min_element(clustering->begin(),clustering->end()));
  for (std::vector<unsigned int>::iterator it = clustering->begin(); it!= clustering->end() ; ++it)
    *it -= min;
boost::shared_ptr<arma::mat> points(new arma::mat(clusterReader.getPointsInMatrix()));
  double killThreshold = 0.0001;
 for (int i = 0 ; i < times ; ++i) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    initAssignRandom(*assignment, points->n_rows, numberOfClusters);
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
    CEC cec(points, assignment, hartigan, killThreshold, numberOfClusters);

    cec.loop();
    std::cout << "Energy " << cec.entropy() << std::endl;
 }
 ASSERT_TRUE(true);
  
}

  void runSpherical(const char * str, int numberOfClusters, int times = 1){
      std::cout.precision(41);
  boost::shared_ptr<std::vector<unsigned int> > clustering(new std::vector<unsigned int>());
  ClusterReader clusterReader(str,4);
  clusterReader.getClustering(*clustering);

  int min = *(std::min_element(clustering->begin(),clustering->end()));
  for (std::vector<unsigned int>::iterator it = clustering->begin(); it!= clustering->end() ; ++it)
    *it -= min;
boost::shared_ptr<arma::mat> points(new arma::mat(clusterReader.getPointsInMatrix()));
  double killThreshold = 0.0001;
      std::vector<ClusterType> types;
    std::vector<float> radius;
    std::vector<arma::mat> covMatrices;
    for (int i = 0 ; i < numberOfClusters ; ++i){
      types.push_back(spherical);
    }
 for (int i = 0 ; i < times ; ++i) {
    boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
    initAssignRandom(*assignment, points->n_rows, numberOfClusters);
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
    CEC cec(points, assignment, hartigan, killThreshold, types,radius,covMatrices);

    cec.loop();
    std::cout << "Energy " << cec.entropy() << std::endl;
 }
 ASSERT_TRUE(true);
  }
TEST(BigData,Normal_1) {
  
  run("bigData_1",3,times);
}


TEST(BigData,Normal_2) {
  
  run("bigData_2",3,times);
}

TEST(BigData,Normal_3) {
  
  run("bigData_3",3,times);
}

TEST(BigData,Normal_4) {
  
  run("bigData_4",3,times);
}

TEST(BigData,Spherical_1) {
  
  runSpherical("bigData_1",3,times);
}


TEST(BigData,Spherical_2) {
  
  runSpherical("bigData_2",3,times);
}

TEST(BigData,Spherical_3) {
  
  runSpherical("bigData_3",3,times);
}

TEST(BigData,Spherical_4) {
  
  runSpherical("bigData_4",3,times);
}
