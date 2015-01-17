#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "random_assignment.hpp"
#include "cec.hpp"
#include "cec_configuration.hpp"
#include "algorithm.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <RcppArmadillo.h>

using namespace gmum;
TEST(EllipseGauss,answer_cluster_same_length) {
    std::vector<unsigned int> clustering;
    std::vector<std::vector<double> > points;

    ClusterReader cluster_reader("EllipseGauss", 2);
    cluster_reader.get_points(points);
    cluster_reader.get_clustering(clustering);
    arma::mat m = cluster_reader.get_points_in_matrix();
    // std::cout << "clustering.size() : " << clustering.size() << std::endl;
    // std::cout << "points.size() : " << points.size() << std::endl;
    EXPECT_EQ(points.size(), clustering.size());
    EXPECT_EQ(points.size(), m.n_rows);
    EXPECT_EQ(points[0].size(), m.n_cols);
    BestPermutationComparator comparator;
    EXPECT_EQ(comparator.evaluate_clustering(4,m,clustering,clustering),1.0);
}
TEST(EllipseGauss,real_test) {
    std::vector<unsigned int> clustering;

    ClusterReader cluster_reader("EllipseGauss", 2);
    boost::shared_ptr<arma::mat> points(
                new arma::mat(cluster_reader.get_points_in_matrix()));
    cluster_reader.get_clustering(clustering);
    BestPermutationComparator comparator;

    Params params;
    params.nclusters = 4;
    params.kill_threshold = 0.0001;
    params.dataset = points;
    params.cluster_type = kstandard;
    params.nstart = 10;

    double number_of_times_acceptable = 0;
    int t = 20;
    for (int x = 0; x < t; ++x) {
        // CEC init
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_method_init("random");
        CecModel cec(&conf);
        // cec.loop();
        SingleResult sr;
        cec.loop();
        double percentage = comparator.evaluate_clustering(params.nclusters, *points, cec.get_assignment(), clustering);
        // std::cout << "Percentage " << percentage << std::endl;
        // EXPECT_GT(percentage, 0.9);
        number_of_times_acceptable += (percentage >= 0.9) || (cec.entropy() < cluster_reader.get_energy()*1.5);

        // EXPECT_LT(cec.entropy(),clusterReader.getEnergy()*1.5);
    }
    EXPECT_GT(number_of_times_acceptable , t/2);
}

