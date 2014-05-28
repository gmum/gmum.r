#include <cmath>
#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
#include "twoeSVMPreprocessor.h"

#include "svm_handler.h"
#include "svm_basic.h"

using namespace Rcpp;



// Below is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar)

// For more on using Rcpp click the Help button on the editor toolbar

//// [[Rcpp::export]]
void TwoeSVMPreprocessor::processRequest( SVMConfiguration data ) {
  // TODO
  return;
}

bool TwoeSVMPreprocessor::canHandle( SVMConfiguration data ) {
  // TODO
return true;
}

arma::mat TwoeSVMPreprocessor::posMat(arma::mat &matrix){
  covPosMat = arma::mat();
  
  for(unsigned int i = 0; i < matrix.n_rows; i++){
    if(matrix(i,matrix.n_cols)==1)covPosMat.insert_rows(i,matrix.submat(i,0,i,matrix.n_cols-1));
  }
  
  return covPosMat;
}


arma::mat TwoeSVMPreprocessor::negMat(arma::mat &matrix){  
  covNegMat = arma::mat();
  
  for(unsigned int i = 0; i < matrix.n_rows; i++){
    if(matrix(i,matrix.n_cols)==-1)covNegMat.insert_rows(i,matrix.submat(i,0,i,matrix.n_cols-1));
  }
  
  return covNegMat;
}



arma::mat TwoeSVMPreprocessor::computeCovPosMat(arma::mat &posMatrix){
  covPosMat= cov(posMatrix);
  return covPosMat;
}

arma::mat TwoeSVMPreprocessor::computeCovNegMat(arma::mat &negMatrix){
  covNegMat= cov(negMatrix);
  return covNegMat;
}

arma::mat TwoeSVMPreprocessor::computeTransMat(arma::mat &covPosMat,arma::mat &covNegMat){
  transMat = covPosMat + covNegMat;
  return transMat;
}

arma::mat TwoeSVMPreprocessor::mappingPos(arma::mat &transMatrix,arma::mat &posMatrix){
arma::mat tmpMatrix;
//arma::mat preprocessorPos;
if(transMatrix.is_square()){
    tmpMatrix = inv(transMatrix);
    tmpMatrix = sqrt(tmpMatrix);
    preprocessorPos = tmpMatrix * posMatrix;
  }
  
  return preprocessorPos;
}

arma::mat TwoeSVMPreprocessor::mappingNeg(arma::mat &transMatrix,arma::mat &negMatrix){
arma::mat tmpMatrix;
arma::mat diagonalMatrix;
float alfa;

if(transMatrix.is_square()){
    diagonalMatrix = arma::mat(transMatrix.n_cols, transMatrix.n_cols, arma::fill::zeros);
    tmpMatrix = inv(transMatrix);
    tmpMatrix = sqrt(tmpMatrix);
    preprocessorNeg = tmpMatrix * negMatrix;
  }
  
  return preprocessorNeg;
}
