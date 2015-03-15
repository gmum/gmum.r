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
#include <armadillo>

using namespace gmum;
TEST(CEC_EllipseGauss,answer_cluster_same_length) {
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
    EXPECT_EQ(comparator.evaluate_clustering(4,clustering,clustering),1.0);
}

#include "boost/date_time/posix_time/posix_time.hpp"

typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;


TEST(CEC_EllipseGauss,real_test) {
    std::vector<unsigned int> clustering;

    ClusterReader cluster_reader("EllipseGauss", 2);

    arma::mat points(cluster_reader.get_points_in_matrix());
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
        //Time t1(boost::posix_time::microsec_clock::local_time());

        //std::cerr<<number_of_times_acceptable<<std::endl;
        // CEC init
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_method_init("random");
        conf.set_algorithm("hartigan");
        //std::cerr<<"Creating"<<std::endl;
        CecModel cec(&conf);
        //std::cerr<<"Created\n"<<std::endl;
        // cec.loop();
        SingleResult sr;
        cec.loop();
        std::vector<unsigned int> a = cec.get_assignment();
        double percentage = comparator.evaluate_clustering(params.nclusters, a, clustering);
        // std::cout << "Percentage " << percentage << std::endl;
        // EXPECT_GT(percentage, 0.9);
        number_of_times_acceptable += (percentage >= 0.9) || (cec.entropy() < cluster_reader.get_energy()*1.5);
        //std::cerr<<number_of_times_acceptable<<std::endl;
        // EXPECT_LT(cec.entropy(),clusterReader.getEnergy()*1.5);

        //Time t2(boost::posix_time::microsec_clock::local_time());
        //TimeDuration dt = t2 - t1;
        //std::cerr << dt << std::endl;
    }
    EXPECT_GT(number_of_times_acceptable , t/2);
}

