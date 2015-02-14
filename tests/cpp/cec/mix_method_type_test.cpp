#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "hartigan.hpp"
#include "random_assignment.hpp"
#include "cec.hpp"
#include "cec_configuration.hpp"
#include "algorithm.hpp"
#include <vector>
#include <armadillo>

using namespace gmum;

TEST(MethodTypeMix, SameAsMethodTypeSphere)
{
    std::vector<unsigned int> clustering;
    ClusterReader cluster_reader("mouse_1_spherical", 2);
    boost::shared_ptr<arma::mat> points(
                new arma::mat(cluster_reader.get_points_in_matrix()));

    cluster_reader.get_clustering(clustering);
    BestPermutationComparator comparator;

    Params params;
    params.nclusters = 3;
    params.kill_threshold = 0.0001;
    params.dataset = points;
    params.nstart = 1;
    params.assignment_type = kkmeanspp;
    params.cluster_type = kmix;
    boost::shared_ptr<ClusterParams> cluster_param = boost::make_shared<ClusterParams>();
    cluster_param->type = ksphere;
    params.clusters.push_back(cluster_param);
    params.clusters.push_back(cluster_param);
    params.clusters.push_back(cluster_param);
    double expected_energy = 3.244395;

    CecConfiguration conf;
    conf.set_params(params);
    CecModel cec(&conf);
    std::vector<unsigned int> a = cec.get_assignment();
    double percentage = comparator.evaluate_clustering(params.nclusters, *points, a, clustering);
    EXPECT_GT(percentage, 0.80);
    EXPECT_NEAR(cec.get_energy(), expected_energy, 1e-5);
}
