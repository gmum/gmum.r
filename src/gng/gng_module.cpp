/*
 * File constructs R Interface, exports necessary classes and functions using Rcpp package
 */

#ifdef RCPP_INTERFACE

#include <RcppArmadillo.h>
using namespace Rcpp;

class GNGConfiguration;
class GNGServer;

RCPP_EXPOSED_CLASS(GNGConfiguration)
RCPP_EXPOSED_CLASS(GNGServer)



#include <gng.h>
#include <gng_server.h>
#include <gng_configuration.h>
using namespace gmum;


GNGServer * loadFromFile(std::string filename){
	GNGServer * out = new GNGServer(filename);
	return out;
}

RCPP_MODULE(gng_module){
	//TODO: Rcpp doesn't accept dot starting name so no way to hide it easily
    Rcpp::function("fromFileGNG", &loadFromFile);


	class_<GNGConfiguration>("GNGConfiguration" )
	.constructor()

	.field(".uniformgrid_optimization", &GNGConfiguration::uniformgrid_optimization, "Uniform grid optimization" )
	.field(".lazyheap_optimization", &GNGConfiguration::lazyheap_optimization )

	.field("alpha", &GNGConfiguration::alpha, "Alpha coefficient. "
			"Decrease the error variables of the nodes neighboring to the newly inserted node by this fraction. Default 0.5")
	.field("beta", &GNGConfiguration::beta, "Beta coefficient. "
			"Decrease the error variables of all node nodes by this fraction. Forgetting rate. Default 0.99")

	.field("eps_n", &GNGConfiguration::eps_n, "How strongly move neighbour node. Default 0.0006")
	.field(".experimental_utility_option", &GNGConfiguration::experimental_utility_option, "Default 0 (off). You can turn it on to 1, but remember to turn off optimizations. Likely will change in the future.")
	.field(".experimental_utility_k",
			&GNGConfiguration::experimental_utility_k, "Default 1.3 (note: option is off by default). ")

	.field("eps_w", &GNGConfiguration::eps_w, "How strongly move winner node. Default 0.05")
	.field("max_edge_age", &GNGConfiguration::max_age, "Max edge age")
	.field("dim", &GNGConfiguration::dim, "Vertex position dimensionality")
	.field("lambda", &GNGConfiguration::lambda, "Every lambda iteration is added new vertex. Default 200")
	.field(".dataset_type", &GNGConfiguration::datasetType, "Dataset type. Currently supported:"
			"2: DatasetBagging - examples are sampled from dataset with equal probability, "
			"3: DatasetBaggingProbability - examples are sampled with probability equal to pos_dim+vertex_dim coordinate (last number in vector)")
	.field("max_nodes", &GNGConfiguration::max_nodes)
	.field("verbosity", &GNGConfiguration::verbosity)
	.field("seed", &GNGConfiguration::seed)
	.field("max_iter", &GNGConfiguration::max_iter)
	.method(".check_correctness", &GNGConfiguration::check_correctness)
	.method(".set_bounding_box", &GNGConfiguration::setBoundingBox)
  .method(".show", &GNGConfiguration::show);

	class_<GNGServer>("GNGServer")
	  .constructor<GNGConfiguration*>()
   .method(".setVerbosity", &GNGServer::setVerbosity)
	 .method(".save", &GNGServer::save)
   .method(".isRunning", &GNGServer::isRunning)
   .method(".run", &GNGServer::run)
	 .method(".pause", &GNGServer::pause)
   .method(".terminate", &GNGServer::terminate)
   .method(".exportToGraphML", &GNGServer::exportToGraphML)
   .method(".getGNGErrorIndex", &GNGServer::getGNGErrorIndex)
   .method(".hasStarted", &GNGServer::hasStarted)
   .method(".nodeDistance", &GNGServer::nodeDistance)
   .method(".insertExamples", &GNGServer::RinsertExamples)
   .method(".insertLabeledExamples", &GNGServer::RinsertLabeledExamples)
   .method(".predict", &GNGServer::Rpredict)
   .method(".getConfiguration", &GNGServer::getConfiguration)
   .method(".getLastNodeIndex", &GNGServer::_getLastNodeIndex)
   .method(".updateClustering", &GNGServer::_updateClustering)
   .method("getClustering", &GNGServer::RgetClustering)
	 .method("getErrorStatistics", &GNGServer::RgetErrorStatistics)
	 .method("getCurrentIteration", &GNGServer::getCurrentIteration)
   .method("getDatasetSize", &GNGServer::getDatasetSize)
   .method("getNumberNodes", &GNGServer::getNumberNodes)
   .method("getNode", &GNGServer::getNode)
   .method("getMeanError", &GNGServer::getMeanError);
}

#include <RcppArmadillo.h>

#endif
