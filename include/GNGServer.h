/* 
 * File:   GNGServer.h
 * Author: staszek
 *
 * Created on October 17, 2013, 8:12 PM
 */

#ifndef GNGSERVER_H
#define	GNGSERVER_H

#include "GNGDefines.h"


#include "GNGConfiguration.h"

#include <cstdlib> //std::system
#include <cstddef>
#include <map>
#include <exception>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <memory>

#include "GNGGlobals.h"
#include "GNGAlgorithmControl.h"
#include "GNGGraph.h"
#include "GNGDatabase.h"
#include "GNGAlgorithm.h"

#include "Utils.h"

/** Holds together all logic and objects.*/
class GNGServer{
    int listening_daemon_sleep_time;
    static GNGConfiguration current_configuration;
    static boost::mutex static_lock;
    static int START_NODES; //TODO: change to 1000!

    /**Construct GNGServer using configuration*/
    GNGServer(GNGConfiguration configuration): listening_daemon_sleep_time(50){
        this->current_configuration = configuration; //assign configuration
        
        #ifdef DEBUG
             dbg.push_back(10, "GNGServer()::constructing GNGServer");
        #endif

        GNG_DIM = current_configuration.dim;

       

        this->shm = std::auto_ptr<SHMemoryManager>(new SHMemoryManager("Server"+to_string<int>(current_configuration.serverId))); 
              
        if(current_configuration.graph_storage == GNGConfiguration::SharedMemory){
         
            #ifdef DEBUG
                 dbg.push_back(10, "GNGServer()::constructing graph storage segment");
            #endif           
            
            this->shm->new_named_segment("GraphStorage",current_configuration.graph_memory_bound);
            
            BoostSHMemoryAllocator * boostSHMemoryAllocator = new BoostSHMemoryAllocator(this->shm->get_named_segment("GraphStorage"));
                      
            SHGNGNode::mm =  boostSHMemoryAllocator;        
            SHGNGNode::alloc_inst = new ShmemAllocatorGNG(this->shm->get_named_segment("GraphStorage")->get_segment_manager());
        }
       
        this->shm->new_named_segment("MessageBufor",current_configuration.message_bufor_size); 
        this->message_bufor_mutex = this->shm->get_named_segment("MessageBufor")->construct<boost::interprocess::interprocess_mutex>("MessageBuforMutex")();
        
        
        this->gngAlgorithmControl = std::auto_ptr<GNGAlgorithmControl>(
                this->shm->get_named_segment("MessageBufor")->construct<GNGAlgorithmControl >("gngAlgorithmControl")()
                );
        
        if(current_configuration.databaseType == GNGConfiguration::DatabaseProbabilistic){
                boost::shared_ptr<std::vector<GNGExampleProbabilistic> > g_database(new std::vector<GNGExampleProbabilistic>());
                this->gngDatabase = std::auto_ptr<GNGDatabase>(new GNGDatabaseProbabilistic<std::vector<GNGExampleProbabilistic> ,boost::mutex>
                        (&this->gngAlgorithmControl->database_mutex_thread, g_database, current_configuration.dim));
        }else if(current_configuration.databaseType == GNGConfiguration::DatabaseSimple){
            throw BasicException("Database type not supported");
        }
        
        #ifdef DEBUG
             dbg.push_back(10, "GNGServer()::gngDatabase and gngAlgorithmicControl constructed");
        #endif       
        if(current_configuration.graph_storage == GNGConfiguration::SharedMemory){
            
            this->gngGraph = std::auto_ptr<SHGNGGraph>(this->shm->get_named_segment("GraphStorage")->
                    construct<SHGNGGraph>("gngGraph")(&this->gngAlgorithmControl->grow_mutex,GNGServer::START_NODES));
            #ifdef DEBUG
                 dbg.push_back(10, "GNGServer()::constructed shared graph");
            #endif
        }
        /* Initiliaze main computing object */
        this->gngAlgorithm = std::auto_ptr<GNGAlgorithm>(new GNGAlgorithm
        (       *this->gngGraph.get(),  //I do not want algorithm to depend on boost
                this->gngDatabase.get(),  
                this->gngAlgorithmControl.get(),
                &current_configuration.orig[0], 
                &current_configuration.axis[0],
                current_configuration.axis[0]/4.0,
                current_configuration.max_nodes,
                current_configuration.max_age,
                current_configuration.alpha,
                current_configuration.beta,
                current_configuration.lambda,
                current_configuration.eps_v,
                current_configuration.eps_n
        ));
        
        #ifdef DEBUG
             dbg.push_back(10, "GNGServer()::constructed algorithm object");
        #endif
        
       
        //TODO: this should be moved to constructor
        this->gngAlgorithm->setToggleLazyHeap(current_configuration.lazyheap_optimization);
        this->gngAlgorithm->setToggleUniformGrid(current_configuration.uniformgrid_optimization);      
    }    
    
    
    /** Run GNG Server - runs in separate thread and returns control
     * @note Runs one extra threads for communication.
     */
    void _run() {
        boost::thread workerThread3(boost::bind(&GNGServer::runSHListeningDaemon, this));

        this->gngAlgorithmControl->setRunningStatus(true); //skrypt w R inicjalizuje

        
        #ifdef DEBUG
        dbg.push_back(10, "GNGServer::run::waiting for database");
        #endif       
        while (this->getDatabase()->getSize() < 200);
        #ifdef DEBUG
        dbg.push_back(10, "GNGServer::run::proceeding to algorithm");
        #endif

        
        
        this->getAlgorithm()->runAlgorithm();
    }
    
    //TODO: use unique_ptr here
    std::auto_ptr<GNGAlgorithmControl>  gngAlgorithmControl;
    std::auto_ptr<GNGAlgorithm> gngAlgorithm;
    std::auto_ptr<SHGNGGraph> gngGraph;
    std::auto_ptr<GNGDatabase> gngDatabase;
    std::auto_ptr<SHMemoryManager> shm;
    /** 
     * Locking communication bufor in interprocess communication
     * Deallocated in destructor 
     **/
    boost::interprocess::interprocess_mutex * message_bufor_mutex; 

    /*Section : protocol handling messages regardless of the source*/
  
    void _handle_AddExamples(double * examples, int count){
        //Handle coding
        if(current_configuration.databaseType==GNGConfiguration::DatabaseProbabilistic)
            for(int i=0;i<count;++i){
                GNGExampleProbabilistic ex(&examples[i*(current_configuration.dim+1)],current_configuration.dim); //decoding
                this->getDatabase()->addExample(&ex);       
            }
        else if(current_configuration.databaseType==GNGConfiguration::DatabaseSimple)
             for(int i=0;i<count;++i){
                GNGExampleProbabilistic ex(&examples[i*current_configuration.dim],current_configuration.dim); //decoding
                this->getDatabase()->addExample(&ex);       
            }       
        else{
            dbg.push_back(100,"Not supported database type" );
            throw BasicException("Not supported database type");
        }
    }
    
public:
    void run() {
        boost::thread workerThread(boost::bind(&GNGServer::_run, this));     
    }    
    static void setConfiguration(GNGConfiguration configuration){
        GNGServer::current_configuration = configuration;
    }   
    static GNGConfiguration getCurrentConfiguration(){
        return GNGServer::current_configuration;
    }
    
    /** Get singleton of GNGServer (thread safe) */
    static GNGServer& getInstance(){
        boost::mutex::scoped_lock sc(static_lock);
        static GNGServer gngServer(current_configuration);
        return gngServer;
    }
    

    

    //TODO: const reference not dandling pointer
    
    GNGAlgorithmControl * getAlgorithmControl(){
        return this->gngAlgorithmControl.get();
    }
    GNGAlgorithm * getAlgorithm(){
        return this->gngAlgorithm.get();
    }
    SHGNGGraph * getGraph(){
        return this->gngGraph.get();
    }
    GNGDatabase * getDatabase(){
        return this->gngDatabase.get();
    }
 
    SHMemoryManager * getSHM(){
        return this->shm.get();
    }    
    
    /**Run main processing loop for shared memory communication channel*/
    void runSHListeningDaemon(){
        #ifdef DEBUG
        dbg.push_back(12, "GNGServer:: runSHListeningDaemon");
        #endif       
        //TODO: add pause checking
        while(true){
           message_bufor_mutex->lock();
           
           SHGNGMessage * current_message = this->getSHM()->get_named_segment("MessageBufor")->find_or_construct<SHGNGMessage>("current_message")();
           int state = current_message->state;
           if(state == SHGNGMessage::Waiting){
                int type = current_message->type;
                
                #ifdef DEBUG
                dbg.push_back(12, "GNGServer::runListeningDaemon caught message of type "+to_string<int>(type));
                #endif

                if(type == SHGNGMessage::AddExamples){
                    SHGNGMessageAddExamples * message_params = this->
                            getSHM()->get_named_segment("MessageBufor")->find<SHGNGMessageAddExamples>("current_message_params").first;
                    
                    
                    if(!message_params){
                        dbg.push_back(100, "GNGServer::runSHListeningDaemon not found message" );
                        throw BasicException("GNGServer::runSHListeningDaemon not found message");
                    }
                    
                    //note - this is quite specific coding 
                    double * examples = this->
                            getSHM()->get_named_segment("MessageBufor")->find<double>(message_params->pointer_reference_name.c_str()).first;
                    
                    if(!examples){
                        dbg.push_back(100, "GNGServer::runSHListeningDaemon not found examples to add" );
                        throw BasicException("GNGServer::runSHListeningDaemon not found examples to add");
                    }
                    
                    _handle_AddExamples(examples, message_params->count);
                    
                    this-> getSHM()->get_named_segment("MessageBufor")->destroy_ptr(examples);
                    this-> getSHM()->get_named_segment("MessageBufor")->destroy_ptr(message_params);
                    
                }
                
                
                current_message->state = SHGNGMessage::Processed;
           }
          
           message_bufor_mutex->unlock();
            
           boost::this_thread::sleep(boost::posix_time::millisec(this->listening_daemon_sleep_time)); 
        }
    }
    
    ~GNGServer(){
        this->shm->get_named_segment("MessageBufor")->destroy_ptr(this->message_bufor_mutex);
    }
    
};

#endif	/* GNGSERVER_H */

