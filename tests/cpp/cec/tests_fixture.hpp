#ifndef TESTS_FIXTURE_H
#define TESTS_FIXTURE_H

#include <gtest/gtest.h>
#include <list>
#include "cluster_reader.hpp"
#include "params.hpp"

struct DefaultGmumParams
{
    DefaultGmumParams(unsigned int nclusters,
                  gmum::ClusterType cluster_type,
                  int it_max = -1);

    gmum::Params gmum_params;
};

struct MixTypeParamsThreeSpheres : public DefaultGmumParams
{
    MixTypeParamsThreeSpheres(int it_max = -1);
};

struct TestsFixtureParam
{
    TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params);

    ClusterReader reader;
    boost::shared_ptr<DefaultGmumParams> default_params;
};

class TestsFixture : public ::testing::TestWithParam< TestsFixtureParam >
{
public:
    TestsFixture();
    virtual ~TestsFixture();

protected:
    std::vector<unsigned int> expected_clustering;
    double expected_energy;
    gmum::Params params;
};

#endif // TESTS_FIXTURE_H
