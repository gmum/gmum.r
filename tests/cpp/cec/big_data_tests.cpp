#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "cec.hpp"
#include "hartigan.hpp"
#include "randomAssignment.hpp"
#include "cecConfiguration.hpp"
#include <boost/smart_ptr.hpp>
#include <vector>

using namespace gmum;

#define SHOW(x) std::cout << #x << " = " << x << std::endl
int times = 10;
void run(const char * str, int numberOfClusters, int times = 1) {
	std::cout.precision(41);
	boost::shared_ptr < std::vector<unsigned int>
			> clustering(new std::vector<unsigned int>());
	ClusterReader clusterReader(str, 4);
	clusterReader.getClustering(*clustering);

	int min = *(std::min_element(clustering->begin(), clustering->end()));
	for (std::vector<unsigned int>::iterator it = clustering->begin();
			it != clustering->end(); ++it)
		*it -= min;
	boost::shared_ptr < arma::mat
			> points(new arma::mat(clusterReader.getPointsInMatrix()));
	Params params;
	params.killThreshold = 0.0001;
	params.nrOfClusters = numberOfClusters;
	params.dataset = points;
	params.clusterType = kstandard;
    params.nstart = 10;

	for (int i = 0; i < times; ++i) {
		cecConfiguration *conf = new cecConfiguration();
		conf->setParams(params);
		conf->setMethodInit("random");
		cecModel cec(conf);
		cec.loop();
		std::cout << "Energy " << cec.entropy() << std::endl;
		delete conf;
	}
	//TODO
	ASSERT_TRUE(true);
}

void runSpherical(const char * str, int numberOfClusters, int times = 1) {
	std::cout.precision(41);
	boost::shared_ptr < std::vector<unsigned int>
			> clustering(new std::vector<unsigned int>());
	ClusterReader clusterReader(str, 4);
	clusterReader.getClustering(*clustering);

	int min = *(std::min_element(clustering->begin(), clustering->end()));
	for (std::vector<unsigned int>::iterator it = clustering->begin();
			it != clustering->end(); ++it)
		*it -= min;
	boost::shared_ptr < arma::mat
			> points(new arma::mat(clusterReader.getPointsInMatrix()));
	Params params;
	params.killThreshold = 0.0001;
	params.nrOfClusters = numberOfClusters;
	params.dataset = points;
	params.clusterType = kstandard;
    params.nstart = 10;

//    std::vector<ClusterType> types;
//    std::vector<float> radius;
//    std::vector<arma::mat> covMatrices;
//    for (int i = 0 ; i < numberOfClusters ; ++i){
//      types.push_back(sphere);
//    }
	for (int i = 0; i < times; ++i) {
		cecConfiguration *conf = new cecConfiguration();
		conf->setParams(params);
		conf->setMethodInit("random");
		cecModel cec(conf);
		cec.loop();
		std::cout << "Energy " << cec.entropy() << std::endl;
		delete conf;
	}
}

TEST(BigData, Normal_1) {
	run("bigData_1", 3, times);
}

TEST(BigData, Normal_2) {
	run("bigData_2", 3, times);
}

TEST(BigData, Normal_3) {
	run("bigData_3", 3, times);
}

TEST(BigData, Normal_4) {
	run("bigData_4", 3, times);
}

TEST(BigData, Spherical_1) {
	runSpherical("bigData_1", 3, times);
}

TEST(BigData, Spherical_2) {
	runSpherical("bigData_2", 3, times);
}

TEST(BigData, Spherical_3) {
	runSpherical("bigData_3", 3, times);
}

TEST(BigData, Spherical_4) {
	runSpherical("bigData_4", 3, times);
}
