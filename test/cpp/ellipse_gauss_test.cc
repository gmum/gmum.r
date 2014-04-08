#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Hartigan.hpp"
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
  //clusterReader.readClustering();
  //clusterReader.readPoints();
  arma::mat m = clusterReader.getPointsInMatrix();
  clusterReader.getClustering(clustering);
  BestPermutationComparator comparator;
  
  for (int x = 0 ; x < 20 ; ++x) {
    int seed = time(NULL);
    srand(seed);
  
    std::vector<unsigned int> fits;
    fits.reserve(clustering.size());

    for(unsigned int i = 0; i < m.n_rows; i++) fits[i] = rand()%numberOfClusters;

    Hartigan algorithm(numberOfClusters,0.0001, fits, m);
    
        algorithm.loop();
    std::vector<unsigned int> result;
    // algorithm.getFits(result);
    //
    // double percentage = comparator.evaluateClustering(numberOfClusters,m,result,clustering);
    double percentage = comparator.evaluateClustering(numberOfClusters,m,fits,clustering);
    std::cout << "Percentage " << percentage << std::endl;
    EXPECT_GT(percentage, 0.9);
  }
}


