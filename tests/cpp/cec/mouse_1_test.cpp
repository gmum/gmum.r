#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "cecConfiguration.hpp"
#include "cec.hpp"
#include <vector>
#include <RcppArmadillo.h>
#include <boost/smart_ptr.hpp>
#include <vector>

using namespace gmum;
class Mouse1Test: public ::testing::Test {
protected:
	Mouse1Test() {
		clustering.reset(new std::vector<unsigned int>());
		ClusterReader clusterReader("mouse_1", 2);
		clusterReader.getClustering(*clustering);
		points.reset(new arma::mat(clusterReader.getPointsInMatrix()));
		energy = clusterReader.getEnergy();
		int min = *(std::min_element(clustering->begin(), clustering->end()));
		for (std::vector<unsigned int>::iterator it = clustering->begin();
				it != clustering->end(); ++it) {
			*it -= min;
		}
		params.nrOfClusters = 3;
		params.killThreshold = 0.0001;
		params.dataset = points;
        params.nstart = 10;
		//params.clusterType = kstandard;
		params.clusterType = ksphere;
		std::cout << "initialized data" << std::endl;
	}
	boost::shared_ptr<std::vector<unsigned int> > clustering;
	boost::shared_ptr<arma::mat> points;
	double energy;
	Params params;
};

#define SHOW_CLUSTERING

TEST_F(Mouse1Test,IsEnergyCorrect) {
	BestPermutationComparator comparator;
	int t = 20;
	int numberOfTimesAcceptable = 0;
	std::cout << "Should get energy : " << energy;
	for (int i = 0; i < t; ++i) {
		cecConfiguration *conf = new cecConfiguration();
		conf->setParams(params);
		conf->setMethodInit("random");
		cecModel cec(conf);
        cec.loop();
        std::vector<unsigned int> assignment = cec.getAssignment();
		double percentage = comparator.evaluateClustering(params.nrOfClusters,*points,assignment,*clustering);
		std::cout << "Percentage " << percentage << std::endl;
		std::cout << "Energy " << cec.entropy() << std::endl;
		numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
#ifdef SHOW_CLUSTERING
		std::cout << "BEGIN" << std::endl;
		for (std::vector<unsigned int>::iterator it = assignment.begin(); it!=assignment.end(); ++it)
		std::cout << *it << std::endl;
		std::cout << "END" << std::endl;
#endif
		delete conf;
	}
	EXPECT_GT(numberOfTimesAcceptable , t/2);
}
