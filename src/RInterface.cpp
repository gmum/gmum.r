/*
 * File constructs R Interface, exports necessary classes and functions using Rcpp package
 */

#define RCPP_INTERFACE
#include <Rcpp.h>
//#include <Rcpp/Module.h>
//#include <armadillo>

#include "GNG.h"
#include "GNGServer.h"

using namespace Rcpp;
using namespace gmum;
DebugCollector dbg;

RCPP_EXPOSED_CLASS(gmum::GNGConfiguration)
RCPP_EXPOSED_CLASS(GNGServer)

void finalizer_of_gng_server(GNGServer * ptr){
	DBG(10, "Called finalizer of GNGServer");
}

RCPP_MODULE(gng_module){
	/// GNGConfiguration class
	class_<gmum::GNGConfiguration>("GNGConfiguration" )
	.constructor()

	.field("uniformgrid_optimization", &gmum::GNGConfiguration::uniformgrid_optimization, "Uniform grid optimization" )
	.field("lazyheap_optimization", &gmum::GNGConfiguration::lazyheap_optimization )
	.method("get_uniform_grid_axis", &gmum::GNGConfiguration::getUniformGridAxis)
	.method("set_uniform_grid_axis", &gmum::GNGConfiguration::setUniformGridAxis)
	.method("get_uniform_grid_origin", &gmum::GNGConfiguration::getUniformGridOrigin)
	.method("set_uniform_grid_origin", &gmum::GNGConfiguration::setUniformGridOrigin)

	.field("alpha", &gmum::GNGConfiguration::alpha, "Alpha coefficient. "
			"Decrease the error variables of the nodes neighboring to the newly inserted node by this fraction. Default 0.5")
	.field("beta", &gmum::GNGConfiguration::beta, "Beta coefficient. "
			"Decrease the error variables of all node nodes by this fraction. Forgetting rate. Default 0.99")

	.field("eps_n", &gmum::GNGConfiguration::eps_n, "Default 0.0006")
	.field("load_graph_filename", &gmum::GNGConfiguration::load_graph_filename, "Default empty. Load serialized graph by serialize_graph method.")
	.field("experimental_utility_option", &gmum::GNGConfiguration::experimental_utility_option, "Default 0 (off). You can turn it on to 1, but remember to turn off optimizations. Likely will change in the future.")
	.field("experimental_utility_k",
			&gmum::GNGConfiguration::experimental_utility_k, "Default 1.3 (note: option is off by default). ")

	.field("eps_v", &gmum::GNGConfiguration::eps_v, "Default 0.05")
	.field("max_edge_age", &gmum::GNGConfiguration::max_age, "Max edge age")
	.field("dim", &gmum::GNGConfiguration::dim, "Vertex position dimensionality")
	.field("lambda", &gmum::GNGConfiguration::lambda, "Every lambda iteration is added new vertex. Default 200")
	.field("dataset_type", &gmum::GNGConfiguration::datasetType, "Dataset type. Currently supported:"
			"2: DatasetBagging - examples are sampled from dataset with equal probability, "
			"3: DatasetBaggingProbability - examples are sampled with probability equal to pos_dim+vertex_dim coordinate (last number in vector)")
	.field("max_nodes", &gmum::GNGConfiguration::max_nodes)
	.method("check_correctness", &gmum::GNGConfiguration::check_correctness);

	class_<GNGServer>("GNGServer").
      constructor<gmum::GNGConfiguration*>()
			.method("s", &GNGServer::getConfiguration)
			.method("dump_memory", &GNGServer::dumpMemory)
			.method("set_memory_move_examples", &GNGServer::RsetExamples, "This function is tricky. It will use"
					"your memory to store examples, so you shouldn't modify the matrix afterwards")
			.method("run", &GNGServer::run)
			.method("pause", &GNGServer::pause)
			.method("dump_graph", &GNGServer::serializeGraph)
			.method("set_debug_level", &GNGServer::setDebugLevel)
			.method("get_configuration", &GNGServer::getConfiguration)
			.method("terminate", &GNGServer::terminate)
			.method("get_mean_error", &GNGServer::getMeanError)
			.method("get_number_nodes", &GNGServer::getNumberNodes)
			.method("export_to_graphml", &GNGServer::exportsToGraphML)
			.method("get_node", &GNGServer::getNode)
			.method("insert_examples", &GNGServer::RinsertExamples)
			.method("get_error_statistics", &GNGServer::getErrorStatistics)
			.method("predict", &GNGServer::Rpredict)
			.finalizer(&finalizer_of_gng_server);
}



