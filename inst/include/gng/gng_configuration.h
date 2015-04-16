/* 
 * File:   GNGConfiguration.h
 * Author: staszek
 *
 * Created on October 17, 2013, 8:11 PM
 */

#ifndef GNGCONFIGURATION_H
#define	 GNGCONFIGURATION_H

#ifdef RCPP_INTERFACE
#include <RcppArmadillo.h>
#include <stdexcept>  
using namespace Rcpp;
#endif

#include "utils/utils.h"
#include "gng_graph.h"
#include <vector>

///Configuration of GNG algorithm/server
struct GNGConfiguration {
	enum GraphNodeStorage {
		NoneGraphNodeStorage, SharedMemory, RAMMemory
	} graph_storage;

	enum DatasetType {
		NoneDatasetTypeinit, DatasetSeq, DatasetSampling, DatasetSamplingProb
	};

	enum ExperimentalUtility {
		UtilityOff, UtilityBasicOn
	};

    int seed;

	/**Maximum number of nodes*/
	int max_nodes; //=1000;
	/**Uniform grid optimization*/
	bool uniformgrid_optimization; //=true,lazyheap=true;
	/**Lazy heap optimization*/
	bool lazyheap_optimization;
	/**Bounding box specification*/

	/**Dimensionality of examples*/
	unsigned dim;

	std::vector<double> orig;
	std::vector<double> axis;
	/**Max edge age*/
	int max_age; //=200;
	/**Alpha coefficient*/
	double alpha; //=0.95;
	/**Beta coefficient*/
	double beta; //=0.9995;
	/**Lambda coefficient*/
	double lambda; //=200;
	/**Epsilion v. How strongly move winning node*/
	double eps_w; //=0.05;
	/**Memory bound*/
	int graph_memory_bound;
	/**Epsilion n*/
	double eps_n; //=0.0006;

    ///Maximum number of iterations
    int max_iter; //=-1;

	int verbosity;

	/**Pseudodistance function used (might be non metric)*/
	int distance_function;

	/**Type of used database, unsgined int for compabititlity with Rcpp**/
	unsigned int datasetType;

	/**Initial reserve memory for nodes */
	int starting_nodes;

	///Utility constant
	double experimental_utility_k;

	///Utility option. Currently supported simples utility
	int experimental_utility_option;

	GNGConfiguration() {
        seed = -1; //is equivalent to null

		verbosity = 1;

        max_iter = -1;

		starting_nodes = 100;

		experimental_utility_option = (int) UtilityOff;
		experimental_utility_k = 1.5;

		graph_storage = RAMMemory;

		dim = 3;
		setBoundingBox(0, 1);

		datasetType = DatasetSampling;
		max_nodes = 1000;
		uniformgrid_optimization = false;
		graph_memory_bound = 200000 * sizeof(double);

		lazyheap_optimization = false;
		max_age = 200;
		alpha = 0.95;
		beta = 0.9995;
		lambda = 200;
		eps_w = 0.05;
		eps_n = 0.0006;

		distance_function = gmum::GNGGraph::Euclidean;

	}

	void deserialize(std::istream & in);

	void serialize(std::ostream & out) const;

    //This is a simplification - we assume square box
	void setBoundingBox(double min, double max) {
		orig = vector<double>();
		axis = vector<double>();
		for (size_t i = 0; i < dim; ++i) {
			orig.push_back(min);
			axis.push_back(max - min);
		}
	}

	/// Get default configuration of GNG Server
	static GNGConfiguration getDefaultConfiguration() {
		GNGConfiguration default_configuration;
		return default_configuration;
	}

    friend ostream & operator<<(ostream & out, const GNGConfiguration & conf){
        conf.serialize(out);
        return out;
    }

    #ifdef RCPP_INTERFACE
    void show() const{
        serialize(cerr);
    }
    #endif

	/// Validate server configuration
    bool check_correctness();
};
#endif	/* GNGCONFIGURATION_H */

