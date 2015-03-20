#include "tests_fixture.hpp"
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
        conf.set_algorithm("hartigan");
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
        conf.set_algorithm("hartigan");
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

INSTANTIATE_TEST_CASE_P(CEC, TestsFixture, ::testing::Values(
    TestsFixtureParam(ClusterReader("mouse_1_spherical", 2), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("mouse_1", 2), boost::make_shared<DefaultGmumParams>(3, ksphere)),
    TestsFixtureParam(ClusterReader("EllipseGauss", 2), boost::make_shared<DefaultGmumParams>(4, kstandard)),
    TestsFixtureParam(ClusterReader("mouse_1_spherical", 2), boost::make_shared<MixTypeParamsThreeSpheres>())
));
