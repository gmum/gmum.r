/*
 * File:   main.cpp
 * Author: Stanislaw "kudkudak" Jastrzebski <staszek [ at ] gmail com>
 *
 * Created on 7 sierpień 2012, 18:27
 */

#include <fstream>
#include <list>
#include <cmath>
#include <iostream>
#include <vector>
#include <iostream>   
#include <utility>   
#include <algorithm>  
#include "GNG.h"

using namespace std;


DebugCollector dbg;

using namespace std;
using namespace boost::interprocess;
using namespace boost;

typedef boost::interprocess::interprocess_mutex MyMutex;

/**
 * 
 * Configuration of GNG algorithm/server
 * TODO: add detailed description for parameters
 */
class GNGConfiguration{
public:
    GNGConfiguration(): gngDim(-1), databaseType(None), serverId(-1){}
    
    enum DatabaseType{
        None,
        DatabaseSimple,
        DatabaseProbabilistic
    };
    /**Maximum number of nodes*/
    int max_nodes;//=1000;
    /**Uniform grid optimization*/
    bool uniformgrid_optimization;//=true,lazyheap=true;
    /**Lazy heap optimization*/
    bool lazyheap_optimization;
    /**Bounding box specification*/
    std::vector<double> orig,axis;
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
    int memory_bound;
    /**Epsilion n*/
    double eps_n;//=0.0006;   
    
    /**Dimensionality of examples*/
    int gngDim;
    /**Type of used database*/
    DatabaseType databaseType;
    /**Id of the server*/
    int serverId;
    
    
    /** Get default configuration of GNG Server */
    static GNGConfiguration getDefaultConfiguration(){
        GNGConfiguration default_configuration;
     
        default_configuration.orig.push_back(0.0);
        default_configuration.orig.push_back(0.0);
        default_configuration.orig.push_back(0.0);
        
        default_configuration.axis.push_back(1.0);
        default_configuration.axis.push_back(1.0);
        default_configuration.axis.push_back(1.0);  
            
        default_configuration.serverId = 0;
        default_configuration.gngDim = 3;
        default_configuration.databaseType = DatabaseSimple;
        default_configuration.max_nodes=1000;       
        default_configuration.uniformgrid_optimization=true; 
        default_configuration.lazyheap_optimization=true;       
        default_configuration.max_age=200;        
        default_configuration.alpha=0.95;     
        default_configuration.beta=0.9995;      
        default_configuration.lambda=200;
        default_configuration.eps_v=0.05;
        default_configuration.eps_n=0.0006;   
        
        return default_configuration;
    }
    
    /**Validate server configuration. *Not working now**/
    bool checkCorrectness(){
        GNGConfiguration empty_configuration;
        return gngDim!=empty_configuration.gngDim && 
               serverId!=empty_configuration.serverId && 
                databaseType != empty_configuration.databaseType;
    }
    
    /**Python like update of configuration*/
    void updateConfiguration(GNGConfiguration update){
        //python dictionary is missing! 
        GNGConfiguration empty_configuration;
        
        if(update.gngDim != empty_configuration.gngDim)
            gngDim = update.gngDim;
        
        if(update.serverId != empty_configuration.serverId)
            serverId = update.serverId;
        
        if(update.databaseType != empty_configuration.databaseType)
            serverId = update.serverId; 
    }
    
};



/** Carries message about message to look for in shared memory used for interprocess communication
 * @Note : We are using shared memory for interprocess communication, not message queue, because storing objects in shared memory is quite convenient
 */
struct SHGNGMessage{
    enum State{
        Waiting,
        Processed
    };
    enum Type{
        GNGExample, /**Interprocess:vector of GNGExample*/
        GNGConfiguration, /**Struct GNGConfiguration*/
        Request /**String, @note: for subset of request we can create more efficient communication protocol*/
    };
};

/** Handles communication with server through different communication channels */
class GNGClient{
    GNGClient(){}
    std::string process_identifier;
public:
    GNGClient(std::string process_identifier): process_identifier(process_identifier){
    
    
    }
};

/** Holds together all logic and objects.*/
class GNGServer{
    int listening_daemon_sleep_time;
    static GNGConfiguration current_configuration;
    static boost::mutex static_lock;
    
public:
    
    static void setConfiguration(GNGConfiguration configuration){
        GNGServer::current_configuration = configuration;
    }    
    static GNGServer& getInstance(){
        boost::mutex::scoped_lock sc(static_lock);
        static GNGServer gngServer(current_configuration);
        return gngServer;
    }
    
    /**OBSOLETE: Create GNGServer (and fill it manually after that)*/
    GNGServer(): listening_daemon_sleep_time(50){}
    /**Construct GNGServer using configuration*/
    GNGServer(GNGConfiguration configuration): listening_daemon_sleep_time(50){
        
        
    }
    
    boost::interprocess::interprocess_mutex communication_bufor_mutex; /** Locking communication bufor in interprocess communication */
    
    GNGAlgorithmControl * gngAlgorithmControl;
    GNGAlgorithm * gngAlgorithm;
    SHGNGGraph * gngGraph;
    GNGDatabase* gngDatabase;
    SHMemoryManager *shm;
    
    GNGAlgorithmControl * getAlgorithmControl(){
        return this->gngAlgorithmControl;
    }
    GNGAlgorithm * getAlgorithm(){
        return this->gngAlgorithm;
    }
    SHGNGGraph * getGraph(){
        return this->gngGraph;
    }
    GNGDatabase * getDatabase(){
        return this->gngDatabase;
    }
 
    SHMemoryManager * getSHM(){
        return this->shm;
    }    
    
    /**Run main processing loop for shared memory communication channel*/
    void runSHListeningDaemon(){
        while(true){
           communication_bufor_mutex.lock();
           
           int status = *this->getSHM()->get_named_segment("MessageBufor")->find<int>("GNGMessageState").first;
           if(status == SHGNGMessage::Waiting){
                int type = *this->getSHM()->get_named_segment("MessageBufor")->find<int>("GNGMessageType").first;
                #ifdef DEBUG
                dbg.push_back(12, "GNGServer::runListeningDaemon caught message of type "+to_string<int>(type));
                #endif

                *this->getSHM()->get_named_segment("MessageBufor")->find<int>("GNGMessageState").first = SHGNGMessage::Processed;
           }
          
           communication_bufor_mutex.unlock();
            
           boost::this_thread::sleep(boost::posix_time::millisec(this->listening_daemon_sleep_time)); 
        }
    }
    
    
};
GNGConfiguration GNGServer::current_configuration = GNGConfiguration::getDefaultConfiguration();
boost::mutex GNGServer::static_lock;


GNGServer * gngServer;

//Global variables
int GNG_DIM, GNG_EX_DIM;

void gngTrainingThread(){
    dbg.set_debug_level(10);
    while(gngServer->gngDatabase->getSize()<2000);
    #ifdef DEBUG
    dbg.push_back(3,"gngTrainingThread::proceeding to algorithm");
    #endif
    gngServer->gngAlgorithm->runAlgorithm();
}


struct Vertex{
	int m_gngid;
};
struct Edge{

	Edge(double w): weight(w),visited_times(0){}
	double weight;
	int visited_times;
};

void gngDatabaseThread(){
    boost::posix_time::millisec workTime(500);
    int k=0;
    double pos[3];

    GNGClient gngClient("Server0");
    

    while(true){
       boost::this_thread::sleep(workTime);
       REPORT(gngServer->gngGraph->getNumberNodes()); 
       REPORT(gngServer->gngAlgorithm->CalculateAccumulatedError());
    }

}


double * uploadOBJ(const char * filename){
    GNGExample ex(3);
    ifstream ifs;
    ifs.open(filename);
    double vertex[3];
    double *bbox = new double[6];
    REP(i,6) bbox[i]=-1;
    std::string v;
    int iteration=0;
    while(!ifs.eof()){
        ifs>>v;
        if(v!="v") continue;
        ++iteration;
        REP(i,3){
            ifs>>vertex[i];
            if(iteration==1){
                bbox[2*i]=vertex[i];
                bbox[2*i+1]=vertex[i];
            }
            else{
                bbox[2*i]=std::min(bbox[2*i],vertex[i]);
                bbox[2*i+1]=std::max(bbox[2*i+1],vertex[i]);
            }
           // write_array(vertex,vertex+3);
           // cout<<bbox[1]<<endl;

        }

    
        gngServer->gngDatabase->addExample(new GNGExample(vertex,3));
    }
    write_array(bbox,bbox+6);
    return bbox;
}
void test_named_segments(){


    SHMemoryManager sh("Server0");
    
   

    sh.new_named_segment("SegmentDatabase", 10000*sizeof(double));
    
    REPORT("Created segment");

    

    sh.get_named_segment("SegmentDatabase")->construct<std::string>("mystring")("Hello");

    REPORT("Created string object");
}

int main(int argc, char** argv) {
    test_named_segments();
 

    //

//    initGNGServer();
	//testDatabase();
    return 0;
}

