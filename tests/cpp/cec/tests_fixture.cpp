#include "tests_fixture.hpp"
#include "cluster_params.hpp"

DefaultGmumParams::DefaultGmumParams(unsigned int nclusters, gmum::ClusterType cluster_type, int it_max)
{
    gmum_params.nclusters = nclusters;
    gmum_params.cluster_type = cluster_type;
    gmum_params.it_max = it_max;
}

MixTypeParamsThreeSpheres::MixTypeParamsThreeSpheres(int it_max)
    :   DefaultGmumParams(3, gmum::kmix, it_max)
{
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::ksphere));
}

TestsFixtureParam::TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, int _times)
    :   reader(_reader), default_params(_params), times(_times)
{ }

TestsFixture::TestsFixture()
{
    TestsFixtureParam p(GetParam());
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 3;
    times = p.times;
}

TestsFixture::~TestsFixture() { }

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
{

}


BigDataTests::BigDataTests()
{
   params.nstart = 10;
   params.it_max = 200;
}

BigDataTests::~BigDataTests()
{

}
