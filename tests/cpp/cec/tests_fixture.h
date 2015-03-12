#ifndef TESTS_FIXTURE_H
#define TESTS_FIXTURE_H

#include <gtest/gtest.h>
#include <list>
#include "cluster_reader.hpp"
#include "params.hpp"

struct DefaultParams
{
    DefaultParams(unsigned int nclusters,
                  gmum::ClusterType cluster_type,
                  int it_max = -1)
    {
        params.nclusters = nclusters;
        params.cluster_type = cluster_type;
        params.it_max = it_max;
    }

    DefaultParams(unsigned int nclusters,
                  gmum::ClusterType cluster_type,
                  std::list< boost::shared_ptr<gmum::ClusterParams> > clusters,
                  int it_max = -1)
    {
        params.nclusters = nclusters;
        params.cluster_type = cluster_type;
        params.it_max = it_max;
        params.clusters = clusters;
    }

    gmum::Params params;
};

struct TestsFixtureParam
{
    TestsFixtureParam(ClusterReader _reader, DefaultParams _params)
        :   reader(_reader), params(_params)
    {
    }

    ClusterReader reader;
    DefaultParams params;
};

class TestsFixture : public ::testing::TestWithParam< TestsFixtureParam >
{
public:
    TestsFixture()
    {
        TestsFixtureParam p(GetParam());
        p.reader.get_clustering(expected_clustering);
        points = p.reader.get_points_in_matrix();
        expected_energy = p.reader.get_energy();
        params = p.params.params;
        params.dataset = boost::make_shared<arma::mat>(points);
    }

    virtual ~TestsFixture() { }

protected:
    arma::mat points;
    std::vector<unsigned int> expected_clustering;
    double expected_energy;
    gmum::Params params;
};

#endif // TESTS_FIXTURE_H
