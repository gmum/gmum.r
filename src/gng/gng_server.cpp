#include <boost/smart_ptr/shared_ptr.hpp>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <string>
#include <utils.h>
#include <gng_node.h>
#include <gng_server.h>
#include <logger.h>

GNGServer::GNGServer(std::string filename) {
	std::ifstream input;
	input.open(filename.c_str(), ios::in | ios::binary);

	GNGConfiguration conf;
	conf.deserialize(input);

	init(conf, &input);
}

GNGServer::GNGServer(GNGConfiguration configuration,
		std::istream * input_graph) {


	init(configuration, input_graph);
}

void GNGServer::init(GNGConfiguration configuration,
		std::istream * input_graph) {

	m_index = gng_server_count++;


	/* If verbosity > 0 and production and using RCPP then logging to file
	 It is because Rcout doesn't work outside of main thread
	 This is first time I used nested macro ifs. This might be refactored to choose during creation like "log_to_file"
	 parameter
	 */
	#ifdef RCPP_INTERFACE
	#ifdef DEBUG
	m_logger = boost::shared_ptr<Logger>(new Logger(configuration.verbosity));
	#else
	if(configuration.verbosity != 0){
		ofstream* log_file = new ofstream();
		log_file->open("gng_" + to_string(m_index) + ".log");
		// TODO: There is leaked resource here
		m_logger = boost::shared_ptr<Logger>(new Logger(configuration.verbosity, *log_file));
	} else {
		m_logger = boost::shared_ptr<Logger>(new Logger(configuration.verbosity));
	}
	#endif
	#else
	m_logger = boost::shared_ptr<Logger>(new Logger(configuration.verbosity));
	#endif

	algorithm_thread = 0;
	m_running_thread_created = false;



    if(configuration.seed != -1){
        LOG_PTR(m_logger, 5, "GNGServer()::seeding to "+to_str(configuration.seed));
        __seed(configuration.seed);
    }

	LOG_PTR(m_logger,5, "GNGServer()::constructing GNGServer");

	if (!configuration.check_correctness()){
		throw invalid_argument("Invalid configuration passed to GNGServer");
    }

	this->current_configuration = configuration; //assign configuration

	if (current_configuration.graph_storage == GNGConfiguration::RAMMemory) {
		//Nothing to do here
	} else {
		throw invalid_argument("Not supported GNGConfiguration type");
	}

	/** Construct database **/
	if (current_configuration.datasetType
			== GNGConfiguration::DatasetSampling) {
		DBG_PTR(m_logger,11, "GNGServer::Constructing Normal Sampling Prob Dataset");
		this->gngDataset = std::auto_ptr<GNGDataset>(
				new GNGDatasetSimple<double>(&database_mutex,
						current_configuration.dim, true /* store_extra */,
						GNGDatasetSimple<double>::Sampling, current_configuration.seed,  m_logger));
	} else if (current_configuration.datasetType
			== GNGConfiguration::DatasetSamplingProb) {
		//Add probability to layout
		DBG_PTR(m_logger,11, "GNGServer::Constructing Sampling Prob Dataset");
		this->gngDataset = std::auto_ptr<GNGDataset>(
				new GNGDatasetSimple<double>(&database_mutex,
						current_configuration.dim, true /* store_extra */,
						GNGDatasetSimple<double>::SamplingProbability, current_configuration.seed, 
						m_logger));
	} else if (current_configuration.datasetType
			== GNGConfiguration::DatasetSeq) {
		DBG_PTR(m_logger,11, "GNGServer::Constructing Normal Seq Dataset");
		this->gngDataset = std::auto_ptr<GNGDataset>(
				new GNGDatasetSimple<double>(&database_mutex,
						current_configuration.dim, true /* store_extra */, 
						GNGDatasetSimple<double>::Sequential, current_configuration.seed, m_logger));
	} else {
		DBG_PTR(m_logger,11, "GNGServer::Not recognized dataset");
		throw BasicException(
				"Database type not supported "
						+ to_string(current_configuration.datasetType));
	}

	DBG_PTR(m_logger,10, "GNGServer()::gngDatabase constructed");

	/** Construct graph **/
	if (current_configuration.graph_storage == GNGConfiguration::SharedMemory) {
		throw BasicException("Not supported SharedMemory configuration");
	} else if (current_configuration.graph_storage
			== GNGConfiguration::RAMMemory) {
		REPORT(current_configuration.starting_nodes);

		this->gngGraph =
				std::auto_ptr<RAMGNGGraph<GNGNode, GNGEdge> >(
						new RAMGNGGraph<GNGNode, GNGEdge>(&grow_mutex,
								current_configuration.dim,
								current_configuration.starting_nodes,
								(gmum::GNGGraph::GNGDistanceFunction) current_configuration.distance_function,
								m_logger));

	} else {
		throw BasicException("Not supported GNGConfiguration type");
	}

	if (input_graph) {
		this->gngGraph->load(*input_graph);
	}

	DBG_PTR(m_logger,10, "GNGServer()::constructing algorithm object");

	/** Initiliaze main computing object **/
	this->gngAlgorithm = std::auto_ptr<GNGAlgorithm>(
			new GNGAlgorithm(
					this->gngGraph.get(), //I do not want algorithm to depend on boost
					this->gngDataset.get(), &current_configuration.orig[0],
					&current_configuration.axis[0],
					current_configuration.axis[0] * 1.1, //only 2^dim //TODO: min
					current_configuration.max_nodes,
					current_configuration.max_age, current_configuration.alpha,
					current_configuration.beta, current_configuration.lambda,
					current_configuration.eps_w, current_configuration.eps_n,
					current_configuration.dim,
					current_configuration.uniformgrid_optimization,
					current_configuration.lazyheap_optimization,
					current_configuration.experimental_utility_option,
					current_configuration.experimental_utility_k, 
                    current_configuration.max_iter,
                    current_configuration.seed, 
                    m_logger));

	DBG_PTR(m_logger,10, "GNGServer()::constructed algorithm object");

}

void GNGServer::run() {
	if(!algorithm_thread){
		DBG_PTR(m_logger,10, "GNGServer::runing algorithm thread");
		algorithm_thread = new gmum::gmum_thread(&GNGServer::_run, (void*) this);
		DBG_PTR(m_logger,10, "GNGServer::runing collect_statistics thread");
		m_running_thread_created = true;
	}else{
		gngAlgorithm->run(/*synchronized*/ true);
	}
}

GNGConfiguration GNGServer::getConfiguration() {
	return current_configuration;
}

bool GNGServer::isRunning() const {
	if (!gngAlgorithm.get()) {
		return false;
	}
	return gngAlgorithm->isRunning();
}

double GNGServer::nodeDistance(int id1, int id2) const {
	if (gngAlgorithm->isRunning()) {
		CERR("nodeDistance: Please pause algorithm before calling nodeDistance function\n");
		return -1.0;
	}
	if (id1 <= 0 || id2 <= 0) {
		CERR("nodeDistance: Indexing starts from 1\n");
		return -1.0;
	}
	return gngGraph->get_dist(id1 - 1, id2 - 1);
}

void GNGServer::save(std::string filename) {
	std::ofstream output;
	output.open(filename.c_str(), ios::out | ios::binary);

	current_configuration.serialize(output);

	try {
		gngGraph->lock();
		ASSERT(filename != "");
		gngGraph->serialize(output);
	} catch (...) {
#ifdef DEBUG_GMUM
		throw BasicException("Failed exporting to GraphML\n");
#endif
		gngGraph->unlock(); //No RAII, yes..
		return;
	}
	gngGraph->unlock();
}

unsigned int GNGServer::getCurrentIteration() const {
	return gngAlgorithm->getIteration();
}

///Exports GNG state to file
void GNGServer::exportToGraphML(std::string filename) {
	try {
		gngGraph->lock();
		ASSERT(filename != "");
		writeToGraphML(getGraph(), filename);
	} catch (...) {
#ifdef DEBUG_GMUM
		throw BasicException("Failed exporting to GraphML\n");
#endif
		gngGraph->unlock(); //No RAII, yes..
		return;
	}
	gngGraph->unlock();
}

///Insert examples
void GNGServer::insertExamples(double * positions, double * extra,
		double * probability, unsigned int count, unsigned int dim) {
	gmum::scoped_lock<GNGDataset> lock(gngDataset.get());

	if (dim != current_configuration.dim) {
		DBG_PTR(m_logger,10, "Wrong dimensionality is "+gmum::to_string(count*dim)+" expected "+
				gmum::to_string(count*gngDataset->getDataDim()) +
				" data dim " + gmum::to_string(gngDataset->size()));
		throw invalid_argument("Wrong dimensionality");
	}

	gngDataset->insertExamples(positions, extra, probability, count);
	DBG_PTR(m_logger,7, "GNGServer::Database size "+gmum::to_string(gngDataset->size()));

}

unsigned int GNGServer::getNumberNodes() const {
	int nr = this->gngGraph->get_number_nodes();
	return nr;
}



double GNGServer::getMeanError() {
	return gngAlgorithm->getMeanError();
}

bool GNGServer::hasStarted() const{
	return this->getCurrentIteration() != 0;
}

vector<double> GNGServer::getMeanErrorStatistics() {
	vector<pair<double, double> > errors =
			gngAlgorithm->getMeanErrorStatistics();
	vector<double> out;
	out.reserve(errors.size());
	for (unsigned i = 0; i < errors.size(); ++i) {
		out.push_back(errors[i].second);
	}
	return out;
}

unsigned GNGServer::getGNGErrorIndex() const{
	return gngAlgorithm->getErrorIndex();
}

#ifdef RCPP_INTERFACE

void GNGServer::_updateClustering(){
	gngAlgorithm->updateClustering();
}

//This is tricky - used only by convertToIGraph in R, because
//it might happen that we delete nodes and have bigger index of the last node
//than actual nodes (especially in the case of utility version of GNG)
unsigned int GNGServer::_getLastNodeIndex() const {
	return gngGraph->get_maximum_index() + 1;
}

///Constructor needed for RCPPInterface
GNGServer::GNGServer(GNGConfiguration * configuration) {
    init(*configuration, 0 /*input_graph*/);
}

///Moderately slow function returning node descriptors
Rcpp::List GNGServer::getNode(int index) {
	int gng_index = index - 1; //1 based

	if(index <= 0) {
		CERR("Indexing of nodes starts from 1 (R convention)\n");
		List ret;
		return ret;
	}

	gngGraph->lock();

	if(!gngGraph->existsNode(gng_index)) {
		List ret;
		return ret;
	}

	GNGNode & n = getGraph()[gng_index];
	NumericVector pos(n.position, n.position + gngDataset->getGNGDim());

	List ret;
	ret["pos"] = pos;
    ret["index"] = index; //useful for graph processing
	ret["error"] = n.error;
	ret["label"] = n.extra_data;

	if(getConfiguration().experimental_utility_option != GNGConfiguration::UtilityOff) {
		ret["utility"] = n.utility;
	}

	vector<unsigned int> neigh(n.size());
	GNGNode::EdgeIterator edg = n.begin();
	unsigned i = 0;
	while(edg!=n.end()) {
		neigh[i++] = (*edg)->nr + 1;
		++edg;
	}

	ret["neighbours"] = IntegerVector(neigh.begin(), neigh.end());

	gngGraph->unlock();

	return ret;
}

int GNGServer::Rpredict(Rcpp::NumericVector & r_ex) {
  if(r_ex.size() > current_configuration.dim){
     CERR("Wrong example dimensionality. Note that C++ method accepts only vectors, not matrix, please use S4 predict method instead\n");
     return -1;
  }else{
	  return 1+gngAlgorithm->predict(std::vector<double>(r_ex.begin(), r_ex.end()) );
  }
}

Rcpp::NumericVector GNGServer::RgetClustering() {
	const vector<int> & x = gngAlgorithm->get_clustering();
	Rcpp::NumericVector out = NumericVector(x.begin(), x.end());
	for(size_t i=0;i<out.size();++i) out[i] += 1; //indexing starts from 1 in R
	return out;
}

Rcpp::NumericVector GNGServer::RgetErrorStatistics() {
	vector<double> x = getMeanErrorStatistics();
	return NumericVector(x.begin(), x.end());
}

void GNGServer::RinsertExamples(Rcpp::NumericMatrix & r_points){
	RinsertLabeledExamples(r_points, Rcpp::NumericVector());
}

void GNGServer::RinsertLabeledExamples(Rcpp::NumericMatrix & r_points,
		Rcpp::NumericVector r_extra ) {
	std::vector<double> extra(r_extra.begin(), r_extra.end());
	arma::mat * points = new arma::mat(r_points.begin(), r_points.nrow(), r_points.ncol(), false);


	arma::Row<double> mean_colwise = arma::mean(*points, 0 /*dim*/);
	arma::Row<double> std_colwise = arma::stddev(*points, 0 /*dim*/);
	arma::Row<double> diff_std = arma::abs(std_colwise - 1.0);
	float max_diff_std = arma::max(diff_std), max_mean = arma::max(mean_colwise);
	if(max_diff_std > 0.1 || max_mean > 0.1) {
		CERR("Warning: it is advised to scale data for optimal algorithm behavior to mean=1 std=0 \n");
	}

	//Check if data fits in bounding box
	if(current_configuration.uniformgrid_optimization) {
		arma::Row<double> max_colwise = arma::max(*points, 0 /*dim*/);
		arma::Row<double> min_colwise = arma::min(*points, 0 /*dim*/);
		arma::Row<double> diff = max_colwise - min_colwise;
		float max = arma::max(diff), min = arma::min(diff);

		for(size_t i=0;i<current_configuration.dim; ++i) {
			if(current_configuration.orig[i] > min_colwise[i] || current_configuration.orig[i]+current_configuration.axis[i] < max_colwise[i]) {
				CERR("Error: each feature has to be in range <min, max> passed to gng.type.optimized \n");
				CERR("Error: returning, did not insert examples\n");
				return;
			}
		}
	}

	arma::inplace_trans( *points, "lowmem");

	if(extra.size()) {
		if(!(points->n_cols== extra.size())){
			CERR("Error: please pass same number of labels as examples\n");
			return;
		}
		insertExamples(points->memptr(), &extra[0], 0 /*probabilty vector*/,
				(unsigned int)points->n_cols, (unsigned int)points->n_rows);
	} else {

		insertExamples(points->memptr(), 0 /* extra vector */, 0 /*probabilty vector*/,
				(unsigned int)points->n_cols, (unsigned int)points->n_rows);
	}

	arma::inplace_trans( *points, "lowmem");

	if(!isRunning()){
		run();
	}
}

#endif

///Pause algorithm
void GNGServer::pause() {
	gngAlgorithm->pause(/* synchronized*/ true);
}

///Terminate algorithm
void GNGServer::terminate() {
	if(gngAlgorithm.get()){
		LOG_PTR(m_logger,5, "GNGServer::getAlgorithm terminating");
		LOG_PTR(m_logger,10, "GNGServer::isRunning ="+ to_str(gngAlgorithm->isRunning()));
		gngAlgorithm->terminate(/*synchronized*/true);
		LOG_PTR(m_logger,10, "GNGServer::isRunning ="+ to_str(gngAlgorithm->isRunning()));
	}
}

GNGAlgorithm & GNGServer::getAlgorithm() {
	return *this->gngAlgorithm.get();
}
GNGGraph & GNGServer::getGraph() {
	return *this->gngGraph.get();
}
GNGDataset & GNGServer::getDatabase() {
	return *this->gngDataset.get();
}

GNGServer::~GNGServer() {
	LOG_PTR(m_logger, 5, "GNGServer::destructor for "+to_str(m_index)+" called");

	if(gngAlgorithm.get()){
	    terminate();
	}

	LOG_PTR(m_logger, 5, "GNGServer::joining to algorithm_thread");

	if(algorithm_thread){
		algorithm_thread->join();
	}



	LOG_PTR(m_logger, 5, "GNGServer::destructor for "+to_str(m_index)+" finished");
}

unsigned GNGServer::getDatasetSize() const{
    if(gngDataset.get()){
	    gmum::scoped_lock<GNGDataset> db_lock(*gngDataset.get());
        return gngDataset->size();
    }else{
        return 0;
    }
}
