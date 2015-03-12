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
    params.nstart = 10;
    params.assignment_type = kkmeanspp;
    params.cluster_type = kmix;
    boost::shared_ptr<ClusterParams> cluster_param = boost::make_shared<ClusterParams>(ksphere);
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
    EXPECT_NEAR(cec.get_energy(), expected_energy, 10e-5);
}

/*
TEST(MethodTypeMix, TestClustersWithParameters)
{
    ClusterReader cluster_reader("mouse_1_spherical", 2);
    boost::shared_ptr<arma::mat> points(
                new arma::mat(cluster_reader.get_points_in_matrix()));

    Params params;
    params.nclusters = 3;
    params.kill_threshold = 0.0001;
    params.dataset = points;
    params.nstart = 10;
    params.assignment_type = kkmeanspp;
    params.cluster_type = kmix;
    boost::shared_ptr<ClusterFullParams> full_cluster_param = boost::make_shared<ClusterFullParams>();
    boost::shared_ptr<ClusterFsphereParams> fsphere_cluster_param = boost::make_shared<ClusterFsphereParams>();

    double cov_mat[] = {0.05, 0, 0, 0.001};

    full_cluster_param->type = kfull;
    full_cluster_param->cov_mat_set = true;
    full_cluster_param->cov_mat = arma::Mat<double>(cov_mat, 2, 2, false);

    fsphere_cluster_param->type = kfsphere;
    fsphere_cluster_param->radius_set = true;
    fsphere_cluster_param->radius = 0.001;

    params.clusters.push_back(full_cluster_param);
    params.clusters.push_back(fsphere_cluster_param);
    params.clusters.push_back(fsphere_cluster_param);
    double expected_energy = 362.635;

    CecConfiguration conf;
    conf.set_params(params);
    CecModel cec(&conf);
    std::vector<unsigned int> a = cec.get_assignment();
    EXPECT_NEAR(cec.get_energy(), expected_energy, 10e-3);
}
*/
