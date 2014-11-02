#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "src/Algorithm.hpp"
#include "src/CEC.hpp"
#include "src/Hartigan.hpp"
#include "src/random_assignment.hpp"
#include <armadillo>
#include <boost/smart_ptr.hpp>
#include <vector>

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

TEST(EllipseGauss,real_test) {
	std::vector<unsigned int> clustering;

	ClusterReader clusterReader("EllipseGauss",2);
	unsigned int numberOfClusters = 4;
	boost::shared_ptr<arma::mat> points(new arma::mat(clusterReader.getPointsInMatrix()));
	clusterReader.getClustering(clustering);
	BestPermutationComparator comparator;

	double numberOfTimesAcceptable = 0;

	int t = 20;
	for (int x = 0; x <t; ++x) {
		boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
		double killThreshold = 0.0001;
		initAssignRandom(*assignment, points->n_rows, numberOfClusters);
		//CEC init
		boost::shared_ptr<Hartigan> hartigan(new Hartigan(false,false));
		CEC cec(points, assignment, hartigan, killThreshold, numberOfClusters);
		// cec.loop();
		SingleResult sr;
		cec.loop();
		double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,clustering);
		std::cout << "Percentage " << percentage << std::endl;
		// EXPECT_GT(percentage, 0.9);
		numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < clusterReader.getEnergy()*1.5);

		//  EXPECT_LT(cec.entropy(),clusterReader.getEnergy()*1.5);

	}
	EXPECT_GT(numberOfTimesAcceptable , t/2);
}

