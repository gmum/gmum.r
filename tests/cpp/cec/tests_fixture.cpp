#include "tests_fixture.hpp"
#include "cluster_params.hpp"

DefaultGmumParams::DefaultGmumParams(unsigned int nclusters, gmum::ClusterType cluster_type, gmum::AssignmentType assignment_type, int it_max)
{
    gmum_params.nclusters = nclusters;
    gmum_params.cluster_type = cluster_type;
    gmum_params.assignment_type = assignment_type;
    gmum_params.it_max = it_max;
}

MixTypeParamsThreeSpheres::MixTypeParamsThreeSpheres(gmum::AssignmentType assignment_type, int it_max)
    :   DefaultGmumParams(3, gmum::kmix, assignment_type, it_max)
{
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
}

TestsFixtureParam::TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _times)
    :   reader(_reader), default_params(_params), times(_times)
{ }

TestsFixture::TestsFixture()
{ }

void TestsFixture::SetUp()
{
    TestsFixtureParam p(GetParam());
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 3;
    times = p.times;
}

TestsFixture::~TestsFixture() { }

BigDataTestsFixtureParam::BigDataTestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _iterations_limit, unsigned int _times)
    :   TestsFixtureParam(_reader, _params, _times), iterations_limit(_iterations_limit)
{
    gmum::Params& params = default_params->gmum_params;
    params.it_max = _iterations_limit + 10;
}
EnergyTests::EnergyTests()
{
    TestsFixtureParam p(GetParam());
    expected_energy = p.reader.get_energy();
}

EnergyTests::~EnergyTests()
{

}


CoverageTests::CoverageTests()
{
    TestsFixtureParam p(GetParam());
    p.reader.get_clustering(expected_clustering);
}

CoverageTests::~CoverageTests()
{ }
BigDataTestsFixture::BigDataTestsFixture()
{ }

void BigDataTestsFixture::SetUp()
{
    BigDataTestsFixtureParam p(GetParam());
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 1;
    iterations_limit = p.iterations_limit;
    times = p.times;
}

BigDataTestsFixture::~BigDataTestsFixture()
{ }

BigDataTests::BigDataTests()
{
   params.nstart = 10;
   params.it_max = 200;
}

BigDataTests::~BigDataTests()
{

}
