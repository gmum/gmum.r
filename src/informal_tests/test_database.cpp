
#include "MemoryAllocator.h"

// 
// void initGNGServer(){
//     gngServer = new GNGServer();
//     gngServer->shm = new SHMemoryManager("Server0",200000000 * sizeof (double)); //nodes <-estimate!
//     gngServer->shm->new_segment(220000000 * sizeof (double)); //database <-estimate!
// 
//     SHGNGNode::mm = gngServer->shm;
//     SHGNGNode::alloc_inst = new ShmemAllocatorGNG(gngServer->shm->get_segment(0)->get_segment_manager());
// 
//     GNG_DIM=3;
// 
//     double orig[3]={-4.0,-4.0,-4.0};
//     double axis[3]={8.0,8.0,8.0};
// 
//     SHGNGExampleDatabaseAllocator alc(gngServer->shm->get_segment(1)->get_segment_manager());
//     SHGNGExampleDatabase * database_vec = 
//             gngServer->shm->get_segment(1)->construct< SHGNGExampleDatabase > ("database_vec")(alc);
// 
//     gngServer->gngAlgorithmControl = gngServer->shm->get_segment(1)->construct<GNGAlgorithmControl >("gngAlgorithmControl")();
// 
//     gngServer->gngDatabase = new GNGDatabaseSimple
//             <SHGNGExampleDatabase, 
//             boost::interprocess::interprocess_mutex>(&gngServer->gngAlgorithmControl->database_mutex,database_vec, 
//     GNG_DIM);
// 
// 
// 
// 
//     gngServer->gngDatabase = new GNGDatabaseMeshes();//new GNGDatabasePlane();//new GNGDatabaseSimple(database_mutex, database_vec);
// 
//     double c1[]={0,1.0,1.0};
//     double c2[]={-1,-2,-1};
//     double c3[]={-1,-1,-1};
//     double c4[]={1,2.10,};
// 
//     GNGDatabaseLine l1(c4,4.0);
// 
//     GNGDatabaseSphere d1(c1, 1.0),d2(c2,1.5);
//     reinterpret_cast<GNGDatabaseMeshes*>(gngServer->gngDatabase)->addMesh(&d1);
//     GNGDatabasePlane p(c3,4.5);
//     reinterpret_cast<GNGDatabaseMeshes*>(gngServer->gngDatabase)->addMesh(&p);
//     reinterpret_cast<GNGDatabaseMeshes*>(gngServer->gngDatabase)->addMesh(&l1);
// 
//     gngServer->gngGraph = gngServer->shm->get_segment(0)->construct<SHGNGGraph>("gngGraph")(&gngServer->gngAlgorithmControl->grow_mutex,25000);
//     gngServer->gngAlgorithm = new GNGAlgorithm
//                 (*gngServer->gngGraph,gngServer->gngDatabase, gngServer->gngAlgorithmControl,
//                 25000, orig, axis,axis[0]/4.0,10000);
// 
//     
//     gngServer->gngAlgorithmControl->setRunningStatus(true); 
//     gngServer->gngAlgorithm->setToggleLazyHeap(false);
//     gngServer->gngAlgorithm->setToggleUniformGrid(false);
// 
//    
//     
//     boost::thread workerThread1(gngTrainingThread);
//     boost::thread workerThread2(gngDatabaseThread);
// 
//     workerThread1.join();
//     workerThread2.join();
// 
// 
//     delete gngServer->shm;
// }
// 
// 
// void testDatabase(){
//     gngServer->shm = new SHMemoryManager("Server0",200000000 * sizeof (double)); //nodes <-estimate!
//     gngServer->shm->new_segment(220000000 * sizeof (double)); //database <-estimate!
// 
//     SHGNGNode::mm = gngServer->shm;
//     SHGNGNode::alloc_inst = new ShmemAllocatorGNG(gngServer->shm->get_segment(0)->get_segment_manager());
// 
//     GNG_DIM=3;
// 
//     double orig[3]={-4.0,-4.0,-4.0};
//     double axis[3]={8.0,8.0,8.0};
// 
//     SHGNGExampleDatabaseAllocator
//     alc(gngServer->shm->get_segment(1)->get_segment_manager());
// 
//     SHGNGExampleDatabase * database_vec = 
//             gngServer->shm->get_segment(1)->construct< SHGNGExampleDatabase > ("database_vec")(alc);
// 
//     gngServer->gngAlgorithmControl = gngServer->shm->get_segment(1)->construct<GNGAlgorithmControl >("gngAlgorithmControl")();
// 
//    gngServer-> gngDatabase = new GNGDatabaseProbabilistic<
//         SHGNGExampleDatabase, 
//         boost::interprocess::interprocess_mutex
//             
//     >
//     (&gngServer->gngAlgorithmControl->database_mutex,database_vec, GNG_DIM);
// 
//     double * pos=new double[4];
//     for(int i=0;i<10000;++i){
//     	pos[0]=__double_rnd(0,1);
//     	pos[1]=__double_rnd(0,1);
//         pos[2]=__double_rnd(0,1);
//         pos[3]=__double_rnd(0,1);
// 
// 
//     	gngServer->gngDatabase->addExample(new GNGExample(pos, 4));
//     }
//     GNGExample ex = gngServer->gngDatabase->drawExample();
//     REPORT(ex);
// 
// 
// 
// 
// }
// 
BoostSHMemoryAllocator * boostSHMemoryAllocator;
 void testLocal(){
     gngServer =new GNGServer();
     
     #ifdef DEBUG
         cout<<"testDatabaseLocal\n";
     #endif
     GNG_DIM=3;
     GNG_EX_DIM=4;
 
     double orig[3]={-4.0,-4.0,-4.0};
     double axis[3]={8.0,8.0,8.0};
 
     
 
     GNG_DIM=3;
     
     std::vector<GNGExample> g_database;
     
     gngServer->shm = new SHMemoryManager("Server0"); //nodes <-estimate!
     gngServer->shm->new_named_segment("GraphStorage",220000000 * sizeof (double)); //database <-estimate!
 
     boostSHMemoryAllocator = new BoostSHMemoryAllocator(gngServer->shm->get_named_segment("GraphStorage"));
     
     SHGNGNode::mm =  boostSHMemoryAllocator;
     SHGNGNode::alloc_inst = new ShmemAllocatorGNG(gngServer->shm->get_named_segment("GraphStorage")->get_segment_manager());
 
      gngServer->gngAlgorithmControl = gngServer->shm->get_named_segment("GraphStorage")->construct<GNGAlgorithmControl >("gngAlgorithmControl")();
 
      gngServer->gngDatabase = new GNGDatabaseProbabilistic<std::vector<GNGExample> ,boost::mutex>
             (&gngServer->gngAlgorithmControl->database_mutex_thread, &g_database, 3);
   
     double * pos=new double[4];
     for(int i=0;i<10000;++i){
     	pos[0]=__double_rnd(0,1);
     	pos[1]=__double_rnd(0,1);
         pos[2]=__double_rnd(0,1);
         pos[3]=__double_rnd(0,1);
         GNGExample * ex = new GNGExample(pos,4);
     	gngServer->gngDatabase->addExample(ex);
         
         
         delete ex;
     }
     REPORT("success database creation");
     GNGExample ex = gngServer->gngDatabase->drawExample();
     REPORT(ex);
 
      gngServer->gngGraph = gngServer->shm->get_named_segment("GraphStorage")->construct<SHGNGGraph>("gngGraph")(&gngServer->gngAlgorithmControl->grow_mutex,25000);
      REPORT("success");
      gngServer->gngAlgorithm = new GNGAlgorithm
                 (* gngServer->gngGraph, gngServer->gngDatabase,  gngServer->gngAlgorithmControl,
                 25000, orig, axis,axis[0]/4.0,10000);
 
      REPORT("success");
     gngServer->gngAlgorithmControl->setRunningStatus(true); //skrypt w R inicjalizuje
     gngServer->gngAlgorithm->setToggleLazyHeap(false);
     gngServer->gngAlgorithm->setToggleUniformGrid(false);
     REPORT("success running main threads");
     boost::thread workerThread1(gngTrainingThread);
 //    boost::thread workerThread3(boost::bind(&GNGServer::runSHListeningDaemon, gngServer));
     boost::thread workerThread2(gngDatabaseThread);
 
     workerThread1.join();
     workerThread2.join();
 //    workerThread3.join();
 
 }


