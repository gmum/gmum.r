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
class Mouse1Test: public ::testing::Test {
protected:
    Mouse1Test() {
        clustering.reset(new std::vector<unsigned int>());
        ClusterReader cluster_reader("mouse_1", 2);
        cluster_reader.get_clustering(*clustering);
        points.reset(new arma::mat(cluster_reader.get_points_in_matrix()));
        energy = cluster_reader.get_energy();
        int min = *(std::min_element(clustering->begin(), clustering->end()));
        for (std::vector<unsigned int>::iterator it = clustering->begin();
             it != clustering->end(); ++it) {
            *it -= min;
        }
        params.nclusters = 3;
        params.kill_threshold = 0.0001;
        params.dataset = points;
        params.nstart = 10;
        //params.clusterType = kstandard;
        params.cluster_type = ksphere;
        // std::cout << "initialized data" << std::endl;
    }
    boost::shared_ptr<std::vector<unsigned int> > clustering;
    boost::shared_ptr<arma::mat> points;
    double energy;
    Params params;
};

TEST_F(Mouse1Test,IsEnergyCorrect) {
    BestPermutationComparator comparator;
    int t = 20;
    int number_of_times_acceptable = 0;
    // std::cout << "Should get energy : " << energy;
    for (int i = 0; i < t; ++i) {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_method_init("random");
        CecModel cec(&conf);
        cec.loop();
        std::vector<unsigned int> assignment = cec.get_assignment();
        double percentage = comparator.evaluate_clustering(params.nclusters,*points,assignment,*clustering);
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
