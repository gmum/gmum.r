/*
* File: GNGExampleManager.h
* Author: Stanisław "kudkudak" Jastrzebski
*
* Created on 11 sierpień 2012, 10:47
*/
#ifndef GNGDATABASE_H
#define GNGDATABASE_H

#include <algorithm>
#include <string.h>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include "SHMemoryManager.h"
#include "Utils.h"
#include "DebugCollector.h"
#include "GNGGlobals.h"
#include "GNGConfiguration.h"

/** Database for growing neural gas interface
*
 * Example is its interiors just an double array with relevant data at different positions.
 * What specifies 
 * 
* @note Drawing example is not very time-expensive comapred to other problems (like
* neighbour search). Therefore it is locking on addExample and drawExample
*
* @note Takes ownership of the memory inserted. Copy memory before inserting if you
* want to use this memory afterwards!!
*/
class GNGDataset
{
public:

    /*
     * @returns Layout for example. We want examples to be laid in continous
     * array of doubles for performance and storage convenience so basically
     * an example can have any dimensionality and this function returns array
     * with specified 3 checkpoints:
     *    * Start of position data
     *    * Start of vertex data (that will be majority voted in algorithm)
     *    * Start of metadata (most likely probability of being sampled)
     */
    virtual std::vector<int> getDataLayout() const=0;
    
    virtual int getDataDim() const=0;
    
    virtual int getGNGDim() const =0;

    ///Returns index of example drawn
    virtual unsigned int drawExample() const=0 ;
    
    ///Retrieves pointer to position 
    virtual const double * getPosition(unsigned int) const=0;
    
    ///Retrieves pointer to metadata, with unsigned int
    virtual const double * getMetaData(unsigned int) const=0;

    ///Retrieves pointer to vertex data, with unsigned int as descriptor of meta
    virtual const double * getVertexData(unsigned int) const=0;
    
    ///Inserts examples to the dataset, @note: Takes ownership of the memory for performance issues
    virtual void insertExamples(void *, unsigned int count, unsigned int size)=0;
    
    virtual void removeExample(unsigned int)=0;
    
    virtual int getSize() const=0;
    
    virtual ~GNGDataset(){}

    GNGDataset(const GNGDataset& orig){}

    GNGDataset(){}

private:

};



/*
 * Abstracts away storage from database double*
 * Possible storages:
 * 
 * * arma matrix
 * * raw memory array
 * * bigmemory backed matrix
 * * csv file (!!)
 * 
 * @note Not supposed to take care of synchronization problems
 */
template<class BaseType, class StorageType>
class GNGDatasetStorage{
protected:
    unsigned int dim_;
public:
    typedef BaseType baseType;
    typedef StorageType storageType;
    
    GNGDatasetStorage(unsigned int dim): dim_(dim)
    {
    }
    
    int dim() const{
        return dim_;
    }    
    
    
    virtual const BaseType * getData(unsigned int) const=0;
 
    virtual unsigned int getSize() const=0;
    ///Add examples to memory, @note Takes ownership of the memory for performance issues
    virtual void addData(StorageType *,  unsigned int, unsigned int) =0;
    ///Will take ownership of x, no copy involved
    virtual void take(StorageType *,  unsigned int) =0;

    virtual ~GNGDatasetStorage(){

    }
};


///Most basic RAM storage
class GNGDatasetStorageRAM: public GNGDatasetStorage<double, double >{
protected:
    double * storage_;
    unsigned int num_examples_;
    bool init_;
    unsigned int storage_size_;
public:
    ///Note that dimensionality is not equal to dimensionality of position pointer
    GNGDatasetStorageRAM(unsigned int dim): 
    	GNGDatasetStorage<double, double >(dim),
    	init_(false), num_examples_(0){
    	storage_size_ = dim_*10;
    	storage_ = new double[storage_size_];


    }
    virtual const double * getData(unsigned int idx) const{
        if(idx*dim_ > storage_size_) return 0;
        return &storage_[idx*dim_];
    }
    virtual void take(double * mem, unsigned int count){
        delete [] storage_; //no auto_ptr because assigment handling is not automatically deleting
        storage_ = mem;
        num_examples_ = count;
        storage_size_ = count*dim_;
    }
    virtual unsigned int getSize() const{
        return storage_size_/dim_;
    }
    
    virtual void addData(double * x, unsigned int count, unsigned int size){
    	assert(size == count*dim_);


        //Always take first batch, do not copy
        if(!init_){
            init_ = true;
            return take(x, count);
        }
        
        
        if((count + num_examples_)*dim_ > storage_size_ ){
            if(storage_size_ < 2e6)
                resize(2*storage_size_);
            else
                resize((unsigned int)(1.2*storage_size_));
        }
        memcpy(storage_, x, sizeof(double)*count*dim_);
        delete[] x;
        num_examples_ += count;
    }
    ~GNGDatasetStorageRAM(){
        delete storage_;
    }
private:
    void resize(unsigned int new_size){
    	//realloc is bad practice in C++, but I want to keep it as double * array
    	//in case we want to use SHM for instance.
    	storage_ = (double*) realloc(storage_, new_size*sizeof(double));
    	storage_size_ = new_size;
    }
};


///Storage :< GNGDatabaseStorage
template<class Storage>
class GNGDatasetSampling: public GNGDataset
{
protected:
    const unsigned int pos_dim_, meta_dim_, vertex_dim_;
    const unsigned int prob_location_;
    
    boost::mutex * mutex_;
    Storage storage_;
    std::vector<int> data_layout_;
    
   
public:

    
    /*
    * @param prob_location If prob location is -1 (by default) means there 
    * is no probability data in meta data. If there is then 
    */
    GNGDatasetSampling(boost::mutex *mutex, unsigned int pos_dim, 
            unsigned int vertex_dim, unsigned int meta_dim, unsigned int prob_location=-1): 
    storage_(pos_dim+vertex_dim+meta_dim),
    mutex_(mutex), pos_dim_(pos_dim), vertex_dim_(meta_dim_), meta_dim_(prob_location),
    prob_location_(prob_location)
    {
        //Data layout 
        data_layout_.push_back(pos_dim_);
        data_layout_.push_back(meta_dim_);
        data_layout_.push_back(vertex_dim_);
        
        __init_rnd();
    }
    

    
     ///Retrieves pointer to position 
    const typename Storage::baseType * getPosition(unsigned int i) const{
        boost::mutex::scoped_lock(*mutex_);
        
        return &storage_.getData(i)[0];
    }
    
    ///Retrieves pointer to metadata, with unsigned int
    const typename Storage::baseType * getMetaData(unsigned int i) const{
        boost::mutex::scoped_lock(*mutex_);
        
        if(meta_dim_==0) return 0;
        
        return &storage_.getData(i)[pos_dim_+vertex_dim_];
    }

    ///Retrieves pointer to vertex data, with unsigned int as descriptor of meta
    const typename Storage::baseType * getVertexData(unsigned int i) const{
        boost::mutex::scoped_lock(*mutex_);
        
        if(vertex_dim_==0) return 0;
        
        return &storage_.getData(i)[pos_dim_];
    }
    
    
    unsigned int drawExample() const{
        boost::mutex::scoped_lock(*mutex_);
        if(prob_location_==-1){
            return __int_rnd(0,storage_.getSize()-1);
        }else{
            const double * ex;
            unsigned int index;
            
            //Sample
            do{ 
             index = __int_rnd(0,storage_.getSize()-1);   
             ex=storage_.getData(index);
            }while(ex[pos_dim_+meta_dim_+vertex_dim_+prob_location_]<__double_rnd(0,1.0));
            
            return index;    
        }
    }
    
    void insertExamples(void * memptr, unsigned int count, unsigned int size){
    	 typename Storage::storageType * examples =
    			reinterpret_cast< typename Storage::storageType *>(memptr);
        boost::mutex::scoped_lock(*mutex_);
        storage_.addData(examples, count, size);
    }
    
    virtual std::vector<int> getDataLayout() const{
    	return data_layout_;
    }
    
    void removeExample(unsigned int index){
        throw 1; //not implemented
    }
    int getSize() const{ 
        boost::mutex::scoped_lock(*mutex_);
        return storage_.getSize();
    } 
    
    virtual int getDataDim() const{
        return pos_dim_+meta_dim_+vertex_dim_;
    }
    
    virtual int getGNGDim() const{
        return pos_dim_;
    }   
private:
};


#endif
/* GNGEXAMPLEMANAGER_H */
