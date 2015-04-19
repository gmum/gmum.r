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
                  gmum::AssignmentType assignment_type = gmum::kkmeanspp,
                  int it_max = -1);

    gmum::Params gmum_params;
};

struct MixTypeParamsThreeSpheres : public DefaultGmumParams
{
    MixTypeParamsThreeSpheres(gmum::AssignmentType assignment_type, int it_max = -1);
};

struct TestsFixtureParam
{
    TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _times);

    ClusterReader reader;
    boost::shared_ptr<DefaultGmumParams> default_params;
    unsigned int times;
};

class TestsFixture : public ::testing::TestWithParam< TestsFixtureParam >
{
public:
    TestsFixture();
    virtual ~TestsFixture();
    virtual void SetUp();
protected:
    gmum::Params params;
    unsigned int times;
};

class EnergyTests : public TestsFixture
struct BigDataTestsFixtureParam : public TestsFixtureParam
{
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
    BigDataTestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _iterations_limit, unsigned int _times = 1);

    unsigned int iterations_limit;
};

class BigDataTestsFixture : public ::testing::TestWithParam< BigDataTestsFixtureParam >
{
public:
    BigDataTestsFixture();
    virtual ~BigDataTestsFixture();
    virtual void SetUp();
protected:
    gmum::Params params;
    unsigned int iterations_limit;
    unsigned int times;

class BigDataTests : public TestsFixture
{
public:
    BigDataTests();
    virtual ~BigDataTests();
};

#endif // TESTS_FIXTURE_H
