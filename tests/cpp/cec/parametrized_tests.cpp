#include "tests_fixture.h"
#include "clustering_comparator.hpp"
#include "cec.hpp"

#include <utility>

using namespace gmum;

TEST_P(TestsFixture, IsEnergyCorrect)
{
    int t = 20;
    int number_of_times_acceptable = 0;

    for (int i = 0; i < t; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        CecModel cec(&conf);
        double diff = std::fabs(cec.get_energy() - expected_energy);

        if( (cec.get_energy() < expected_energy) || (diff <= 10e-5) )
        {
            ++number_of_times_acceptable;
        }
    }
    EXPECT_GT(number_of_times_acceptable , t / 2.0);
}

TEST_P(TestsFixture, IsCoverageCorrect)
{
    BestPermutationComparator comparator;
    int t = 20;
    int number_of_times_acceptable = 0;

    for (int i = 0; i < t; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        CecModel cec(&conf);
        std::vector<unsigned int> clustering = cec.get_assignment();
        double percentage = comparator.evaluate_clustering(params.nclusters, expected_clustering, clustering);

        if( percentage >= 0.9 )
        {
            ++number_of_times_acceptable;
        }
    }
    EXPECT_GT(number_of_times_acceptable , t / 2.0);
}

TEST_P(BigDataTestsFixture, BigDataTest)
{
    for (int i = 0; i < times; ++i)
    {
            CecConfiguration conf;
            conf.set_params(params);
            CecModel cec(&conf);
            std::cout << cec.iters() << " <= " << iterations_limit << std::endl;
            ASSERT_LE(cec.iters(), iterations_limit);
    }
}

INSTANTIATE_TEST_CASE_P(CEC_NumericTest, BigDataTestsFixture, ::testing::Values(
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), DefaultParams(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), DefaultParams(3, ksphere), 25),

    BigDataTestsFixtureParam(ClusterReader("bigData_2"), DefaultParams(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_2"), DefaultParams(3, ksphere), 25),

    BigDataTestsFixtureParam(ClusterReader("bigData_3"), DefaultParams(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_3"), DefaultParams(3, ksphere), 25),

    BigDataTestsFixtureParam(ClusterReader("bigData_4"), DefaultParams(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_4"), DefaultParams(3, ksphere), 25)
));

INSTANTIATE_TEST_CASE_P(CEC, TestsFixture, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), DefaultParams(3, ksphere)),
    TestsFixtureParam(ClusterReader("mouse_1"), DefaultParams(3, ksphere)),
    TestsFixtureParam(ClusterReader("EllipseGauss"), DefaultParams(4, kstandard))
    // TestsFixtureParam(ClusterReader("mouse_1_spherical", 2), DefaultParams(3, kmix, {boost::make_shared<ClusterParams>(ksphere), boost::make_shared<ClusterParams>(ksphere), boost::make_shared<ClusterParams>(ksphere)}))
));
