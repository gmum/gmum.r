#include "gtest/gtest.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <armadillo>
#include <vector>
#include "src/Cluster.hpp"


TEST(OnlineFormulas,AddPoint){
  
  int n = 100;
  int dim = 2;
  int beg = dim+1;
  double acceptableDifference = 1e-10;
  
  std::vector<unsigned int> fits;
  unsigned id = 1;
  arma::mat data(n,dim);
  data.randu();
  for (int i = 0 ; i < n ; ++i) {
    fits.push_back(id);
  }
  
  arma::mat initMatrix(beg,dim);
  for(int k = 0 ; k < beg ; ++k){
    for (int j = 0 ; j < dim; ++j){
      initMatrix(k,j) = data(k,j);
    }
  }
  
  
  Cluster m = Cluster(id,fits,initMatrix);
  // Dodajemy element o indeksie i  
  for (int i = beg ; i < n-1 ; ++i){
    
    arma::mat tmpMatrix(i+1,dim);
    for (int j = 0 ; j <=i; ++j){
      tmpMatrix.row(j) = data.row(j);
    }
    arma::mat covariance= cov(tmpMatrix,1);
    arma::mat realM = mean(tmpMatrix);


    arma::rowvec point(data.row(i));
    m = m.addPoint(point);
    Cluster tmp(id,fits,tmpMatrix);
    arma::rowvec  meanOnlineDifference = m.getMean() - realM;
    arma::mat meanInitDifference = realM - tmp.getMean();
    arma::mat covOnlineDifference = m.getCovMat() - covariance;
    arma::mat covInitDifference = covariance - tmp.getCovMat();
  
    EXPECT_EQ(m.size(),tmp.size());
    
    for (int j = 0 ; j < dim; ++j){
      EXPECT_LT(std::abs(meanOnlineDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
      EXPECT_LT(std::abs(meanInitDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
      
    }
    
     for (int j = 0 ; j < dim; ++j){
       for (int k = 0; k < dim ; ++k){
	 EXPECT_LT(std::abs(covOnlineDifference(j,k)),acceptableDifference ) << " at position (" << j << "," << k <<  ")" << "differs by more than " << acceptableDifference ;
	 	 EXPECT_LT(std::abs(covInitDifference(j,k)),acceptableDifference) << " at position (" << j << "," << k <<  ")" << "differs by more than " << acceptableDifference ;
	 
      }
     }
 
   }  
  
}

TEST(OnlineFormulas,removePoint){
  
  int n = 100;
  int dim = 2;
  int end = dim+1;
  double acceptableDifference = 1e-10;
  
  std::vector<unsigned int> fits;
  unsigned id = 1;
  arma::mat data(n,dim);
  data.randu();
  for (int i = 0 ; i < n ; ++i) {
    fits.push_back(id);
  }
  
  arma::mat initMatrix(n,dim);
  for(int k = 0 ; k < n ; ++k){
    for (int j = 0 ; j < dim; ++j){
      initMatrix(k,j) = data(k,j);
    }
  }
  
  
  Cluster m = Cluster(id,fits,initMatrix);
  // Dodajemy element o indeksie i  
  for (int i = n-1 ; i > end ; --i){
    
    arma::mat tmpMatrix(i,dim);
    for (int j = 0 ; j <i; ++j){
      tmpMatrix.row(j) = data.row(j);
    }
    arma::mat covariance= cov(tmpMatrix,1);
    arma::mat realM = mean(tmpMatrix);


    arma::rowvec point(data.row(i));
    m = m.removePoint(point);
    Cluster tmp(id,fits,tmpMatrix);
    arma::rowvec  meanOnlineDifference = m.getMean() - realM;
    arma::mat meanInitDifference = realM - tmp.getMean();
    arma::mat covOnlineDifference = m.getCovMat() - covariance;
    arma::mat covInitDifference = covariance - tmp.getCovMat();
  
    EXPECT_EQ(m.size(),tmp.size());
    
    for (int j = 0 ; j < dim; ++j){
      EXPECT_LT(std::abs(meanOnlineDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
      EXPECT_LT(std::abs(meanInitDifference(j)),acceptableDifference) << "at position" << j << " means differ by more than " << acceptableDifference;
      
    }
    
     for (int j = 0 ; j < dim; ++j){
       for (int k = 0; k < dim ; ++k){
	 EXPECT_LT(std::abs(covOnlineDifference(j,k)),acceptableDifference ) << " at position (" << j << "," << k <<  ")" << "differs by more than " << acceptableDifference ;
	 	 EXPECT_LT(std::abs(covInitDifference(j,k)),acceptableDifference) << " at position (" << j << "," << k <<  ")" << "differs by more than " << acceptableDifference ;
	 
      }
     }
 
   }  
  
}

