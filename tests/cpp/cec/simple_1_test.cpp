#include "gtest/gtest.h"
#include "cluster_reader.hpp"
#include "clustering_comparator.hpp"
#include "cec_configuration.hpp"
#include "hartigan.hpp"
#include "cec.hpp"
#include <vector>
#include <RcppArmadillo.h>
#include <boost/smart_ptr.hpp>
#include <vector>
using namespace gmum;

#define SHOW(x) std::cout << #x << " = " << x << std::endl
TEST(Simple_1,IsEnergyCorrect) {
    // std::cout.precision(21);
    boost::shared_ptr<std::vector<unsigned int> > clustering(new std::vector<unsigned int>);
    ClusterReader cluster_reader("simple_1",2);
    cluster_reader.get_clustering(*clustering);

    int min = *(std::min_element(clustering->begin(),clustering->end()));
    for (std::vector<unsigned int>::iterator it = clustering->begin(); it!= clustering->end(); ++it)
        *it -= min;

    boost::shared_ptr<arma::mat> points(new arma::mat(cluster_reader.get_points_in_matrix()));
    Params params;
    params.kill_threshold = 0.0001;
    params.nclusters = 1;
    params.dataset = points;
    params.cluster_type = kstandard;
    params.nstart = 10;

    CecConfiguration conf;
    conf.set_params(params);
    conf.set_method_init("random");
    // TODO set clustering
    //conf->setMix(*clustering);
    CecModel cec(&conf);

    // std::cout << cec.entropy() << std::endl;
    // std::cout << cluster_reader.get_energy() << std::endl;
    // std::cout << " mean : " << cec.get_clusters()[0]->get_mean() << std::endl;
    // SHOW(cec.get_clusters()[0]->get_mean().n_rows );
    // SHOW(cec.get_clusters()[0]->get_mean().n_cols);
    // std::cout << cluster_reader.get_energy() << std::endl;
    EXPECT_LT(std::abs(cec.entropy() - cluster_reader.get_energy()) , 1e-4);
}
