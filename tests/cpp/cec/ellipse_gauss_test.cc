#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "cec.hpp"
#include "algorithm.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <armadillo>
using namespace gmum;
TEST(EllipseGauss,answer_cluster_same_length) {
  std::vector<unsigned int> clustering;
  std::vector<std::vector<double> > points;
 
  ClusterReader clusterReader("EllipseGauss",2);
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

#define EVAL(x) std::cout << #x << "() = " << x() << std::endl
#define SHOW(x) std::cout << #x << " = " << x << std::endl


TEST(EllipseGauss,real_test){ 
  std::vector<unsigned int> clustering;
  
  ClusterReader clusterReader("EllipseGauss",2);
  boost::shared_ptr<arma::mat> points(new arma::mat(clusterReader.getPointsInMatrix()));
  clusterReader.getClustering(clustering);
  BestPermutationComparator comparator;

  Params params;
  params.nrOfClusters = 4;
  params.killThreshold = 0.0001;
  params.dataset = points;

  double numberOfTimesAcceptable = 0;
  int t = 20;
  for (int x = 0 ; x <t ; ++x) {
    boost::shared_ptr<std::vector<unsigned int> > assignment (new std::vector<unsigned int>);

    RandomAssignment randomAssignment(*points, params.nrOfClusters);
    assignment->resize(params.dataset->n_rows);
    randomAssignment(*assignment);
    //CEC init
    boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
    CEC cec(hartigan, assignment, params);
    // cec.loop();
    SingleResult sr;
    cec.loop();
    double percentage = comparator.evaluateClustering(params.nrOfClusters,*points,*assignment,clustering);
    std::cout << "Percentage " << percentage << std::endl;
    // EXPECT_GT(percentage, 0.9);
    numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < clusterReader.getEnergy()*1.5);
    
    //  EXPECT_LT(cec.entropy(),clusterReader.getEnergy()*1.5);

  }
  EXPECT_GT(numberOfTimesAcceptable , t/2);
}


