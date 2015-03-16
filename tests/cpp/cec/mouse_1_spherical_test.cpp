#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "random_assignment.hpp"
#include "cec_configuration.hpp"
#include "cec.hpp"
#include <vector>
#include <armadillo>
#include <boost/smart_ptr.hpp>
#include <vector>

using namespace gmum;
class CEC_Mouse1SphericalTest: public ::testing::Test {
protected:
    CEC_Mouse1SphericalTest() {
        clustering.reset(new std::vector<unsigned int>());
        ClusterReader cluster_reader("mouse_1_spherical", 2);
        cluster_reader.get_clustering(*clustering);
        points = arma::mat(cluster_reader.get_points_in_matrix());
        energy = cluster_reader.get_energy();
        params.nclusters = 3;
        params.kill_threshold = 0.0001;
        params.dataset = points;
        params.cluster_type = ksphere;
        params.nstart = 10;
        // std::cout << "initialized data" << std::endl;
    }
    boost::shared_ptr<std::vector<unsigned int> > clustering;
    arma::mat points;
    double energy;
    Params params;
};

TEST_F(CEC_Mouse1SphericalTest,IsEnergyCorrect) {
    BestPermutationComparator comparator;
    int t = 20;
    int number_of_times_acceptable = 0;
    // std::cout << "Should get energy : " << energy;
    for (int i = 0; i < t; ++i) {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_method_init("random");
        conf.set_algorithm("hartigan");
        CecModel cec(&conf);
        cec.loop();
        std::vector<unsigned int> assignment = cec.get_assignment();
        double percentage = comparator.evaluate_clustering(params.nclusters,assignment,*clustering);
        // std::cout << "Percentage " << percentage << std::endl;
        // std::cout << "Energy " << cec.entropy() << std::endl;
        number_of_times_acceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
#ifdef SHOW_CLUSTERING
        std::cout << "BEGIN" << std::endl;
        for (std::vector<unsigned int>::iterator it = assignment.begin(); it!=assignment.end(); ++it)
            std::cout << *it << std::endl;
        std::cout << "END" << std::endl;
#endif
    }
    EXPECT_GT(number_of_times_acceptable , t/2);
}

TEST_F(CEC_Mouse1SphericalTest,StartingFromCorrectAssignment) {
    BestPermutationComparator comparator;
    int t = 1;
    int number_of_times_acceptable = 0;
    // std::cout << "Should get energy : " << energy;
    for (int i = 0; i < t; ++i) {
        boost::shared_ptr<std::vector<unsigned int> > assignment(new std::vector<unsigned int>());
        for (std::vector<unsigned int>::iterator it = clustering->begin();it!=clustering->end(); ++it)
            assignment->push_back(*it);
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_method_init("random");
        conf.set_algorithm("hartigan");
        CecModel cec(&conf);
        // cec.setAssignment(assignment); //is it ok?
        cec.loop();
        double percentage = comparator.evaluate_clustering(params.nclusters,*assignment,*clustering);
        // std::cout << "Percentage " << percentage << std::endl;
        // std::cout << "Energy " << cec.entropy() << std::endl;
        number_of_times_acceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
#ifdef SHOW_CLUSTERING
        std::cout << "BEGIN" << std::endl;
        for (std::vector<unsigned int>::iterator it = assignment->begin(); it!=assignment->end(); ++it)
            std::cout << *it << std::endl;
        std::cout << "END" << std::endl;
#endif
    }
    EXPECT_GT(number_of_times_acceptable , t ==1 ? 0.9 : t/2);
}
//TEST_F(Mouse1SphericalTest,NormalIsEnergyCorrect) {
//	double killThreshold = 0.0001;
//	BestPermutationComparator comparator;
//	int t = 20;
//	int numberOfTimesAcceptable = 0;
//	// std::cout << "Should get energy : " << energy;
//	for (int i = 0; i < t; ++i) {
//		cecConfiguration *conf = new cecConfiguration();
//		conf->setParams(params);
//		conf->setMethodInit("random");
//		cecModel cec(conf);
//		cec.loop();
//		double percentage = comparator.evaluateClustering(numberOfClusters,*points,*assignment,*clustering);
//		// std::cout << "Percentage " << percentage << std::endl;
//		// std::cout << "Energy " << cec.entropy() << std::endl;
//		numberOfTimesAcceptable += (percentage >= 0.9) || (cec.entropy() < energy*1.5);
//#ifdef SHOW_CLUSTERING
//		// std::cout << "BEGIN" << std::endl;
//		for (std::vector<unsigned int>::iterator it = assignment->begin(); it!=assignment->end(); ++it)
//		// std::cout << *it << std::endl;
//		// std::cout << "END" << std::endl;
//#endif
//		delete conf;
//	}
//	EXPECT_GT(numberOfTimesAcceptable , t/2);
//}
