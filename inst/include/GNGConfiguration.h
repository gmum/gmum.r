/* 
 * File:   GNGConfiguration.h
 * Author: staszek
 *
 * Created on October 17, 2013, 8:11 PM
 */

#ifndef GNGCONFIGURATION_H
#define	 GNGCONFIGURATION_H

//#ifdef RCPP_INTERFACE

#include <Rcpp.h>
#include <armadillo>


using namespace Rcpp;
using namespace arma;

//#endif

#include <vector>

/**
 * 
 * Configuration of GNG algorithm/server
 * TODO: add detailed description for parameters
 */
class GNGConfiguration{
public:
	GNGConfiguration(){
		//Not reusing code because of peculiar problem with Rcpp

		orig.clear();

		axis.clear();

        dataset_vertex_dim = 0;

        starting_nodes = 100;

        message_bufor_size = 10000*sizeof(double);

        orig.push_back(0.0);
        orig.push_back(0.0);
        orig.push_back(0.0);

        axis.push_back(1.0);
        axis.push_back(1.0);
        axis.push_back(1.0);

        graph_storage = RAMMemory;

        serverId = 0;
        dim = 3;
        datasetType = DatasetSampling;
        max_nodes=1000;
        uniformgrid_optimization=false;
        graph_memory_bound = 200000*sizeof(double);

        lazyheap_optimization=false;
        max_age=200;
        alpha=0.5;
        beta=0.99;
        lambda=200;
        eps_v=0.05;
        eps_n=0.0006;

        interprocess_communication = false;
	}




//#ifdef RCPP_INTERFACE
//	GNGConfiguration(SEXP _xp){
//		//Rcpp::XPtr<GNGConfiguration> ptr(_xp);
////		operator=(as<GNGConfiguration>(_xp));
//
//	}

	void setUniformGridAxis(NumericVector v){
		axis.clear();
		for(int i=0;i<v.size();++i){
			axis.push_back(v(i));
		}
	}
	NumericVector getUniformGridAxis(){
		return NumericVector(axis.begin(), axis.end());
	}
	void setUniformGridOrigin(NumericVector v){
		orig.clear();
		for(int i=0;i<v.size();++i){
			orig.push_back(v(i));
		}
	}

	NumericVector getUniformGridOrigin(){
		return NumericVector(orig.begin(), orig.end());
	}
//#endif


    enum GraphNodeStorage{
        NoneGraphNodeStorage,
        SharedMemory,
        RAMMemory
    } graph_storage;

  
    enum DatasetType{
        NoneDatasetType,
        DatasetSeq,
        DatasetSampling,
        DatasetSamplingProb
    };
    

    
    int message_bufor_size;
    /**Maximum number of nodes*/
    int max_nodes;//=1000;
    /**Uniform grid optimization*/
    bool uniformgrid_optimization;//=true,lazyheap=true;
    /**Lazy heap optimization*/
    bool lazyheap_optimization;
    /**Bounding box specification*/
    std::vector<double> orig;
    std::vector<double> axis;
    /**Max edge age*/
    int max_age;//=200;
    /**Alpha coefficient*/
    double alpha;//=0.95;
    /**Beta coefficient*/
    double beta;//=0.9995;
    /**Lambda coefficient*/
    double lambda;//=200;
    /**Epsilion v*/
    double eps_v;//=0.05;
    /**Memory bound*/
    int graph_memory_bound;
    /**Epsilion n*/
    double eps_n;//=0.0006;   
    
    /**Dimensionality of examples*/
    int dim;
    /**Type of used database, unsgined int for compabititlity with Rcpp**/
    unsigned int datasetType;
    /**Id of the server*/
    int serverId;
    
    /**Initial reserve memory for nodes */
    int starting_nodes;
    
    bool interprocess_communication; /**< Should server listen for incommin connection from other processes? Not possible in the current version */
   

    
    /** Dataset layout */
    
    ///Dimensionality of vertex extra data (will be possible to vote on this data among vertices)
    unsigned int dataset_vertex_dim;
    
    
    /** Get default configuration of GNG Server */
    static GNGConfiguration getDefaultConfiguration(){
        GNGConfiguration default_configuration;
        
        default_configuration.orig.clear();

        default_configuration.axis.clear();

        default_configuration.dataset_vertex_dim = 0;

        default_configuration.starting_nodes = 100;
        
        default_configuration.message_bufor_size = 10000*sizeof(double);
        
        default_configuration.orig.push_back(0.0);
        default_configuration.orig.push_back(0.0);
        default_configuration.orig.push_back(0.0);
        
        default_configuration.axis.push_back(1.0);
        default_configuration.axis.push_back(1.0);
        default_configuration.axis.push_back(1.0);  
            
        default_configuration.graph_storage = RAMMemory;
        
        default_configuration.serverId = 0;
        default_configuration.dim = 3;
        default_configuration.datasetType = DatasetSampling;
        default_configuration.max_nodes=1000;       
        default_configuration.uniformgrid_optimization=false; 
        default_configuration.graph_memory_bound = 200000*sizeof(double);
        
        default_configuration.lazyheap_optimization=false;       
        default_configuration.max_age=200;        
        default_configuration.alpha=0.95;     
        default_configuration.beta=0.9995;      
        default_configuration.lambda=200;
        default_configuration.eps_v=0.05;
        default_configuration.eps_n=0.0006; 
        
        default_configuration.interprocess_communication = false;

        
        return default_configuration;
    }
    
    /**Validate server configuration. *Not working now**/
    bool check_correctness(){
    	if(datasetType > 3 or datasetType <= 0){
    		cerr<<"ERROR: wrong database specified\n";
    		DBG(20, "ERROR: wrong database specified\n");
    		return false;
    	}
    	if(! (dim < 20 || ! uniformgrid_optimization)){
    		DBG(20, "ERROR: Too big dimensionality for uniformgrid_optimization");
    		cerr<<"ERROR: Too big dimensionality for uniformgrid_optimization\n";
    		return false;
    	}
    	if(! (!uniformgrid_optimization or (dim == axis.size() && dim == orig.size()))){
    		DBG(20, "ERROR: dimensionality doesn't agree with axis and orig");
    		cerr<<"ERROR: dimensionality doesn't agree with axis and orig"<<endl;
    		return false;
    	}

        return true;
    }
//    
//    /**Python like update of configuration*/
//    void updateConfiguration(GNGConfiguration update){
//        //python dictionary is missing! 
//        GNGConfiguration empty_configuration;
//        
//        if(update.gngDim != empty_configuration.gngDim)
//            gngDim = update.gngDim;
//        
//        if(update.serverId != empty_configuration.serverId)
//            serverId = update.serverId;
//        
//        if(update.databaseType != empty_configuration.databaseType)
//            serverId = update.serverId; 
//    }
    
};

#endif	/* GNGCONFIGURATION_H */

