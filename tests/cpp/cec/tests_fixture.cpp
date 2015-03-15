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

TestsFixtureParam::TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params)
    :   reader(_reader), default_params(_params)
{ }

TestsFixture::TestsFixture()
{
    TestsFixtureParam p(GetParam());
    p.reader.get_clustering(expected_clustering);
    expected_energy = p.reader.get_energy();
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 3;
}

TestsFixture::~TestsFixture() { }




