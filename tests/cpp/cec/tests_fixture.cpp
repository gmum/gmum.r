#include "cec_configuration.hpp"
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
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::kspherical));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::kspherical));
    gmum_params.clusters.push_back(boost::make_shared<gmum::ClusterParams>(gmum::kspherical));
}

TestsFixtureParam::TestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _times)
    :   reader(_reader), default_params(_params), times(_times)
{ }

BigDataTestsFixtureParam::BigDataTestsFixtureParam(ClusterReader _reader, boost::shared_ptr<DefaultGmumParams> _params, unsigned int _iterations_limit, unsigned int _times)
    :   TestsFixtureParam(_reader, _params, _times), iterations_limit(_iterations_limit)
{
    gmum::Params& params = default_params->gmum_params;
    params.it_max = _iterations_limit + 10;
}

void TestsFixture::SetUp()
{
    TestsFixtureParam p(GetParam());
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 3;
    times = p.times;
    seed = 13371337;
}

void BigDataTests::SetUp()
{
    BigDataTestsFixtureParam p(GetParam());
    params = p.default_params->gmum_params;
    params.dataset = p.reader.get_points_in_matrix();
    params.nstart = 1;
    iterations_limit = p.iterations_limit;
    times = p.times;
    seed = 13371337;
}

void CoverageTests::SetUp()
{
    TestsFixture::SetUp();
    TestsFixtureParam p(GetParam());
    p.reader.get_clustering(expected_clustering);
}

void EnergyTests::SetUp()
{
    TestsFixture::SetUp();
    TestsFixtureParam p(GetParam());
    expected_energy = p.reader.get_energy();
}

void ControlEpsTests::SetUp()
{
    TestsFixture::SetUp();
    params.kill_threshold = (params.dataset.n_rows - 1) / static_cast<double>(params.dataset.n_rows);
    params.log_nclusters = true;
}



