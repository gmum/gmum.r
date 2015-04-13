#include "tests_fixture.hpp"
#include "clustering_comparator.hpp"
#include "cec.hpp"

#include <utility>

using namespace gmum;

TEST_P(TestsFixture, IsEnergyCorrect)
{
    int number_of_times_acceptable = 0;

    for (unsigned int i = 0; i < times; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_algorithm("hartigan");
        CecModel cec(&conf);
        double diff = std::fabs(cec.get_energy() - expected_energy);

        if( (cec.get_energy() < expected_energy) || (diff <= 10e-5) )
        {
            ++number_of_times_acceptable;
        }
    }
    EXPECT_GT(number_of_times_acceptable , times / 2.0);
}

TEST_P(TestsFixture, IsCoverageCorrect)
{
    BestPermutationComparator comparator;
    int number_of_times_acceptable = 0;

    for (unsigned int i = 0; i < times; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_algorithm("hartigan");
        CecModel cec(&conf);
        std::vector<unsigned int> clustering = cec.get_assignment();
        double percentage = comparator.evaluate_clustering(params.nclusters, expected_clustering, clustering);

        if( percentage >= 0.9 )
        {
            ++number_of_times_acceptable;
        }
    }
    EXPECT_GT(number_of_times_acceptable , times / 2.0);
}

TEST_P(BigDataTestsFixture, BigDataTest)
{
    for (unsigned int i = 0; i < times; ++i)
    {
            CecConfiguration conf;
            conf.set_params(params);
            conf.set_algorithm("hartigan");
            CecModel cec(&conf);
            ASSERT_LE(cec.iters(), iterations_limit);
    }
}

TEST_P(TestsFixture, IsControlEpsBoundaryCaseCorrect)
{
    CecConfiguration conf;
    params.kill_threshold = (params.dataset.n_rows - 1) / static_cast<double>(params.dataset.n_rows);
    params.log_nclusters = true;
    conf.set_params(params);
    conf.set_algorithm("hartigan");
    CecModel cec(&conf);
    int final_nclusters = cec.get_nclusters().back();
    EXPECT_EQ(final_nclusters, 1);
}

/*
INSTANTIATE_TEST_CASE_P(CEC_NumericTest, BigDataTestsFixture, ::testing::Values(
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), boost::make_shared<DefaultGmumParams>(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), boost::make_shared<DefaultGmumParams>(3, ksphere), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_2"), boost::make_shared<DefaultGmumParams>(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_2"), boost::make_shared<DefaultGmumParams>(3, ksphere), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_3"), boost::make_shared<DefaultGmumParams>(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_3"), boost::make_shared<DefaultGmumParams>(3, ksphere), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_4"), boost::make_shared<DefaultGmumParams>(3, kstandard), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_4"), boost::make_shared<DefaultGmumParams>(3, ksphere), 25)
));
*/

INSTANTIATE_TEST_CASE_P(CEC, TestsFixture, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical", 2), boost::make_shared<DefaultGmumParams>(3, ksphere), 20),
    TestsFixtureParam(ClusterReader("mouse_1", 2), boost::make_shared<DefaultGmumParams>(3, ksphere), 20),
    TestsFixtureParam(ClusterReader("EllipseGauss", 2), boost::make_shared<DefaultGmumParams>(4, kstandard), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical", 2), boost::make_shared<MixTypeParamsThreeSpheres>(), 20)
));
