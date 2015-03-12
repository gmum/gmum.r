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
                  int it_max = -1);

    DefaultParams(unsigned int nclusters,
                  gmum::ClusterType cluster_type,
                  std::list< boost::shared_ptr<gmum::ClusterParams> > clusters,
                  int it_max = -1);

    gmum::Params params;
};

struct TestsFixtureParam
{
    TestsFixtureParam(ClusterReader _reader, DefaultParams _params);

    ClusterReader reader;
    DefaultParams params;
};

class TestsFixture : public ::testing::TestWithParam< TestsFixtureParam >
{
public:
    TestsFixture();
    virtual ~TestsFixture();

protected:
    arma::mat points;
    std::vector<unsigned int> expected_clustering;
    double expected_energy;
    gmum::Params params;
};

#endif // TESTS_FIXTURE_H
