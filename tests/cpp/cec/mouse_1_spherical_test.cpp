#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "random_assignment.hpp"
#include "cec_configuration.hpp"
#include "cec.hpp"
#include <vector>
#include <RcppArmadillo.h>
#include <boost/smart_ptr.hpp>
#include <vector>

using namespace gmum;
class Mouse1SphericalTest: public ::testing::Test {
protected:
	Mouse1SphericalTest() {
		clustering.reset(new std::vector<unsigned int>());
		ClusterReader clusterReader("mouse_1_spherical", 2);
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
		params.clusterType = ksphere;
        params.nstart = 10;
		std::cout << "initialized data" << std::endl;
	}
	boost::shared_ptr<std::vector<unsigned int> > clustering;
	boost::shared_ptr<arma::mat> points;
	double energy;
	Params params;
};

#define SHOW_CLUSTERING

TEST_F(Mouse1SphericalTest,IsEnergyCorrect) {
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

TEST_F(Mouse1SphericalTest,StartingFromCorrectAssignment) {
	BestPermutationComparator comparator;
	int t = 1;
	int numberOfTimesAcceptable = 0;
	std::cout << "Should get energy : " << energy;
	for (int i = 0; i < t; ++i) {
		boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
		for (std::vector<unsigned int>::iterator it = clustering->begin();it!=clustering->end(); ++it)
		assignment->push_back(*it);
		cecConfiguration *conf = new cecConfiguration();
		conf->setParams(params);
		conf->setMethodInit("random");
		cecModel cec(conf);
		// cec.setAssignment(assignment); //is it ok?
		cec.loop();
		double percentage = comparator.evaluateClustering(params.nrOfClusters,*points,*assignment,*clustering);
		std::cout << "Percentage " << percentage << std::endl;
		std::cout << "Energy " << cec.entropy() << std::endl;
		numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
#ifdef SHOW_CLUSTERING
		std::cout << "BEGIN" << std::endl;
		for (std::vector<unsigned int>::iterator it = assignment->begin(); it!=assignment->end(); ++it)
		std::cout << *it << std::endl;
		std::cout << "END" << std::endl;
#endif
		delete conf;
	}
	EXPECT_GT(numberOfTimesAcceptable , t ==1 ? 0.9 : t/2);
}
//TEST_F(Mouse1SphericalTest,NormalIsEnergyCorrect) {
//	double killThreshold = 0.0001;
//	BestPermutationComparator comparator;
//	int t = 20;
//	int numberOfTimesAcceptable = 0;
//	std::cout << "Should get energy : " << energy;
//	for (int i = 0; i < t; ++i) {
//		cecConfiguration *conf = new cecConfiguration();
//		conf->setParams(params);
//		conf->setMethodInit("random");
//		cecModel cec(conf);
//		cec.loop();
//		double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
//		std::cout << "Percentage " << percentage << std::endl;
//		std::cout << "Energy " << cec.entropy() << std::endl;
//		numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
//#ifdef SHOW_CLUSTERING
//		std::cout << "BEGIN" << std::endl;
//		for (std::vector<unsigned int>::iterator it = assignment->begin(); it!=assignment->end(); ++it)
//		std::cout << *it << std::endl;
//		std::cout << "END" << std::endl;
//#endif
//		delete conf;
//	}
//	EXPECT_GT(numberOfTimesAcceptable , t/2);
//}
