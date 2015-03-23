#include "tests_fixture.hpp"
#include "clustering_comparator.hpp"
#include "cec.hpp"

#include <utility>

using namespace gmum;

TEST_P(EnergyTests, IsEnergyCorrect)
{
    int number_of_times_acceptable = 0;
    for (int i = 0; i < times; ++i)
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

TEST_P(CoverageTests, IsCoverageCorrect)
{
    BestPermutationComparator comparator;
    int number_of_times_acceptable = 0;
    for (int i = 0; i < times; ++i)
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

TEST_P(BigDataTests, IsEnergyNan)
{
    for (int i = 0; i < times; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_algorithm("hartigan");
        CecModel cec(&conf);
        ASSERT_FALSE(std::isnan(cec.get_energy()));
    }
}

INSTANTIATE_TEST_CASE_P(CEC, EnergyTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard)),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>())
));

INSTANTIATE_TEST_CASE_P(CEC, CoverageTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard)),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>())
));

/*
INSTANTIATE_TEST_CASE_P(CEC_NumericTest, BigDataTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("DimSets_32"), boost::make_shared<DefaultGmumParams>(16, ksphere), 1),
    TestsFixtureParam(ClusterReader("DimSets_32"), boost::make_shared<DefaultGmumParams>(16, kstandard), 1),
    TestsFixtureParam(ClusterReader("DimSets_32"), boost::make_shared<DefaultGmumParams>(16, kdiagonal), 1),
    TestsFixtureParam(ClusterReader("DimSets_64"), boost::make_shared<DefaultGmumParams>(16, ksphere), 1),
    TestsFixtureParam(ClusterReader("DimSets_64"), boost::make_shared<DefaultGmumParams>(16, kstandard), 1),
    TestsFixtureParam(ClusterReader("DimSets_64"), boost::make_shared<DefaultGmumParams>(16, kdiagonal), 1)
));
*/
