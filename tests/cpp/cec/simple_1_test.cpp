#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "cec_configuration.hpp"
#include "hartigan.hpp"
#include "cec.hpp"
#include <vector>
#include <RcppArmadillo.h>
#include <boost/smart_ptr.hpp>
#include <vector>
using namespace gmum;

#define SHOW(x) std::cout << #x << " = " << x << std::endl
TEST(Simple_1,IsEnergyCorrect) {
	std::cout.precision(21);
	boost::shared_ptr<std::vector<unsigned int> > clustering(new std::vector<unsigned int>);
	ClusterReader clusterReader("simple_1",2);
	clusterReader.getClustering(*clustering);

	int min = *(std::min_element(clustering->begin(),clustering->end()));
	for (std::vector<unsigned int>::iterator it = clustering->begin(); it!= clustering->end(); ++it)
	*it -= min;

	boost::shared_ptr<arma::mat> points(new arma::mat(clusterReader.getPointsInMatrix()));
	Params params;
	params.killThreshold = 0.0001;
	params.nrOfClusters = 1;
	params.dataset = points;
	params.clusterType = kstandard;
    params.nstart = 10;

	cecConfiguration *conf = new cecConfiguration();
	conf->setParams(params);
    conf->setMethodInit("random");
	// TODO set clustering
	//conf->setMix(*clustering);
	cecModel cec(conf);

	std::cout << cec.entropy() << std::endl;
	std::cout << clusterReader.getEnergy() << std::endl;
	std::cout << " mean : " << cec.clusters[0]->getMean() << std::endl;
	SHOW(cec.clusters[0]->getMean().n_rows );
	SHOW(cec.clusters[0]->getMean().n_cols);
	std::cout << clusterReader.getEnergy() << std::endl;
	EXPECT_LT(std::abs(cec.entropy() - clusterReader.getEnergy()) , 1e-4);
}
