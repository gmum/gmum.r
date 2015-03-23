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
    TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, int _times = 20);

    ClusterReader reader;
    boost::shared_ptr<DefaultGmumParams> default_params;
    int times;
};

class TestsFixture : public ::testing::TestWithParam< TestsFixtureParam >
{
public:
    TestsFixture();
    virtual ~TestsFixture();

protected:
    gmum::Params params;
    int times;
};

class EnergyTests : public TestsFixture
{
public:
    EnergyTests();
    virtual ~EnergyTests();
protected:
    double expected_energy;
};

class CoverageTests : public TestsFixture
{
public:
    CoverageTests();
    virtual ~CoverageTests();
protected:
    std::vector<unsigned int> expected_clustering;
};

class BigDataTests : public TestsFixture
{
public:
    BigDataTests();
    virtual ~BigDataTests();
};

#endif // TESTS_FIXTURE_H
