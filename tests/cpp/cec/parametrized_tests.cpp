#include "tests_fixture.hpp"
#include "clustering_comparator.hpp"
#include "cec.hpp"

#include <utility>

using namespace gmum;

TEST_P(EnergyTests, IsEnergyCorrect)
{
    int number_of_times_acceptable = 0;

    for (unsigned int i = 0; i < times; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_algorithm("hartigan");
        conf.set_seed(seed);
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

    for (unsigned int i = 0; i < times; ++i)
    {
        CecConfiguration conf;
        conf.set_params(params);
        conf.set_algorithm("hartigan");
        conf.set_seed(seed);
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

TEST_P(BigDataTests, BigDataTest)
{
    for (unsigned int i = 0; i < times; ++i)
    {
            CecConfiguration conf;
            conf.set_params(params);
            conf.set_algorithm("hartigan");
            conf.set_seed(seed);
            CecModel cec(&conf);
            ASSERT_LE(cec.iters(), iterations_limit);
    }
}

TEST_P(ControlEpsTests, IsControlEpsBoundaryCaseCorrect)
{
    CecConfiguration conf;
    conf.set_params(params);
    conf.set_algorithm("hartigan");
    conf.set_seed(seed);
    CecModel cec(&conf);
    int final_nclusters = cec.get_nclusters().back();
    EXPECT_EQ(final_nclusters, 1);
}

INSTANTIATE_TEST_CASE_P(CEC_NumericTest, BigDataTests, ::testing::Values(
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), boost::make_shared<DefaultGmumParams>(3, kstandard, kkmeanspp), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_1"), boost::make_shared<DefaultGmumParams>(3, kstandard, krandom), 45),
                                                                                
    BigDataTestsFixtureParam(ClusterReader("bigData_2"), boost::make_shared<DefaultGmumParams>(3, kstandard, kkmeanspp), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_2"), boost::make_shared<DefaultGmumParams>(3, kstandard, krandom), 45),
                                                                                
    BigDataTestsFixtureParam(ClusterReader("bigData_3"), boost::make_shared<DefaultGmumParams>(3, kstandard, kkmeanspp), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_3"), boost::make_shared<DefaultGmumParams>(3, kstandard, krandom), 45),
                                                                                
    BigDataTestsFixtureParam(ClusterReader("bigData_4"), boost::make_shared<DefaultGmumParams>(3, kstandard, kkmeanspp), 25),
    BigDataTestsFixtureParam(ClusterReader("bigData_4"), boost::make_shared<DefaultGmumParams>(3, kstandard, krandom), 45)
));

INSTANTIATE_TEST_CASE_P(CEC, EnergyTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, krandom), 20),

    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, krandom), 20),
    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, kkmeanspp), 20),

    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(krandom), 20)
));

INSTANTIATE_TEST_CASE_P(CEC, CoverageTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, krandom), 20),

    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, krandom), 20),
    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, kkmeanspp), 20),

    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(krandom), 20)
));

INSTANTIATE_TEST_CASE_P(CEC, ControlEpsTests, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1"), boost::make_shared<DefaultGmumParams>(3, kspherical, krandom), 20),

    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("EllipseGauss"), boost::make_shared<DefaultGmumParams>(4, kstandard, krandom), 20),

    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, krandom), 20),
    TestsFixtureParam(ClusterReader("simple_1"), boost::make_shared<DefaultGmumParams>(1, kstandard, kkmeanspp), 20),

    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(kkmeanspp), 20),
    TestsFixtureParam(ClusterReader("mouse_1_spherical"), boost::make_shared<MixTypeParamsThreeSpheres>(krandom), 20)
));
