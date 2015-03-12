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
                  int it_max = 25)
    {
        params.nclusters = nclusters;
        params.cluster_type = cluster_type;
        params.it_max = it_max;
        params.nstart = 10;
    }

    DefaultParams(unsigned int nclusters,
                  gmum::ClusterType cluster_type,
                  std::list< boost::shared_ptr<gmum::ClusterParams> > clusters,
                  int it_max = 25)
    {
        params.nclusters = nclusters;
        params.cluster_type = cluster_type;
        params.it_max = it_max;
        params.clusters = clusters;
        params.nstart = 10;
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

struct BigDataTestsFixtureParam : public TestsFixtureParam
{
    BigDataTestsFixtureParam(ClusterReader _reader, DefaultParams _params, unsigned int _iterations_limit, unsigned int _times = 10)
        :   TestsFixtureParam(_reader, _params), iterations_limit(_iterations_limit), times(_times)
    {
        params.params.it_max = _iterations_limit + 10;
        params.params.nstart = 1;
    }
    unsigned int iterations_limit;
    unsigned int times;
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

class BigDataTestsFixture : public ::testing::TestWithParam< BigDataTestsFixtureParam >
{
public:
    BigDataTestsFixture()
    {
        BigDataTestsFixtureParam p(GetParam());
        points = p.reader.get_points_in_matrix();
        params = p.params.params;
        params.dataset = boost::make_shared<arma::mat>(points);
        times = p.times;
        iterations_limit = p.iterations_limit;
    }

    virtual ~BigDataTestsFixture() { }
protected:
    arma::mat points;
    gmum::Params params;
    int times;
    unsigned int iterations_limit;
};


#endif // TESTS_FIXTURE_H
