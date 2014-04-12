#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
#include "src/random_assignment.hpp"
#include "src/CEC.hpp"
#include <vector>

TEST(EllipseGauss,answer_cluster_same_length) {
  std::vector<unsigned int> clustering;
  std::vector<std::vector<double> > points;
 
  ClusterReader clusterReader("EllipseGauss",2);
  //clusterReader.readPoints();
  //clusterReader.readClustering();
  clusterReader.getPoints(points);
  clusterReader.getClustering(clustering);
  arma::mat m = clusterReader.getPointsInMatrix();
  std::cout << "clustering.size() : " << clustering.size() << std::endl;
  std::cout << "points.size() : " << points.size() << std::endl;
  EXPECT_EQ(points.size(), clustering.size());
  EXPECT_EQ(points.size(), m.n_rows);
  EXPECT_EQ(points[0].size(), m.n_cols);
  BestPermutationComparator comparator;
  EXPECT_EQ(comparator.evaluateClustering(4,m,clustering,clustering),1.0);
  
}

TEST(EllipseGauss,real_test){ 
  std::vector<unsigned int> clustering;
  
  ClusterReader clusterReader("EllipseGauss",2);
  unsigned int numberOfClusters = 4;
  arma::mat points = clusterReader.getPointsInMatrix();
  clusterReader.getClustering(clustering);
  BestPermutationComparator comparator;
  for (int x = 0 ; x < 20 ; ++x) {
    std::vector<unsigned int> assignment;
    double killThreshold = 0.0001;
    initAssignRandom(assignment, points.n_rows, numberOfClusters);
    //CEC init
    CEC *cec;
    Hartigan hartigan;
    cec = new CEC(points, assignment, killThreshold, &hartigan,numberOfClusters);
    cec->loop();
    double percentage = comparator.evaluateClustering(numberOfClusters,points,assignment,clustering);
    std::cout << "Percentage " << percentage << std::endl;
    EXPECT_GT(percentage, 0.9);
  }
}


