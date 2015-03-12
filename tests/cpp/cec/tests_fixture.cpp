#include "tests_fixture.hpp"

DefaultParams::DefaultParams(unsigned int nclusters, gmum::ClusterType cluster_type, int it_max)
{
    params.nclusters = nclusters;
    params.cluster_type = cluster_type;
    params.it_max = it_max;
}

DefaultParams::DefaultParams(unsigned int nclusters, gmum::ClusterType cluster_type, std::list<boost::shared_ptr<gmum::ClusterParams> > clusters, int it_max)
{
    params.nclusters = nclusters;
    params.cluster_type = cluster_type;
    params.it_max = it_max;
    params.clusters = clusters;
}

TestsFixtureParam::TestsFixtureParam(ClusterReader _reader, DefaultParams _params)
    :   reader(_reader), params(_params)
{ }

TestsFixture::TestsFixture()
{
    TestsFixtureParam p(GetParam());
    p.reader.get_clustering(expected_clustering);
    points = p.reader.get_points_in_matrix();
    expected_energy = p.reader.get_energy();
    params = p.params.params;
    params.dataset = boost::make_shared<arma::mat>(points);
}

TestsFixture::~TestsFixture() { }
