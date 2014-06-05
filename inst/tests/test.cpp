#include <iostream>
#include "armadillo"

using namespace arma;
using namespace std;


arma::mat covPosMat;
arma::mat covNegMat;
	
//Preprocessor
arma::mat posMat(arma::mat &matrix);
arma::mat negMat(arma::mat &matrix);
arma::mat sqrtMat(arma::mat &matrix);
arma::mat computeCovPosMat(arma::mat &posMatrix);
arma::mat computeCovNegMat(arma::mat &negMatrix);
arma::mat computeTransMat(arma::mat &covPosMat,arma::mat &covNegMat);
arma::mat mappingPos(arma::mat &transMatrix,arma::mat &posMatrix);
arma::mat mappingNeg(arma::mat &transMatrix,arma::mat &negMatrix);
//Postprocessor
arma::rowvec projectingDataPos(arma::mat &matrix,arma::rowvec &weights);

int main(int argc, char** argv)
  {
  cout << "Armadillo version: " << arma_version::as_string() << endl;
 
mat A;

  A << 0.555950 << 0.274690 << 0.540605 << 1.0 << endr
    << 0.108929 << 0.830123 << 0.891726 << 1.0 << endr
    << 0.948014 << 0.973234 << 0.216504 << -1.0 << endr
    << 0.023787 << 0.675382 << 0.231751 << 1.0 << endr
	<< 0.555950 << 0.274690 << 0.540605 << -1.0 << endr
    << 0.108929 << 0.830123 << 0.891726 << 1.0 << endr
    << 0.948014 << 0.973234 << 0.216504 << -1.0 << endr
    << 0.023787 << 0.675382 << 0.231751 << 1.0 << endr
	<< 0.555950 << 0.274690 << 0.540605 << -1.0 << endr
    << 0.108929 << 0.830123 << 0.891726 << 1.0 << endr
    << 0.948014 << 0.973234 << 0.216504 << -1.0 << endr
    << 0.023787 << 0.675382 << 0.231751 << -1.0 << endr;

cout<<"Size"<<endl;
cout<<"cols "<<A.n_cols<<endl;
cout<<"rows "<<A.n_rows<<endl;

A.print();
cout<<"Pos and Neg matrixs"<<endl<<endl;

posMat(A).print();

cout<<"============="<<endl;

negMat(A).print();


cout<<"Cov matrix: "<<endl<<endl;

arma::mat pos = computeCovPosMat(posMat(A));
pos.print();
cout<<"========="<<endl<<endl;
arma::mat neg = computeCovNegMat(negMat(A));
neg.print();
cout<<"Trans Matrix: "<<endl;

computeTransMat(pos,neg).print();


cout<<"Root of matrix example form Wiki: "<<endl<<endl;
mat T;
T<<33.0<<24.0<<endr
 <<48.0<<57.0<<endr;
T.print();

cout<<"Root: "<<endl<<endl;
sqrtMat(T).print();

cout<<"All together!!! : "<<endl;
//arma::mat &transMatrix,arma::mat &posMatrix

cout<<"Mapping for positive: "<<endl;

mappingPos(computeTransMat(pos,neg),posMat(A)).print();

cout<<"Mapping for negative: "<<endl;

mappingNeg(computeTransMat(computeCovPosMat(posMat(A)),computeCovNegMat(posMat(A))),negMat(A)).print();


  system("Pause");
  return 0;
  }

arma::mat posMat(arma::mat &matrix) {
     arma::mat covPosMat;
	 int insertion = 0;
    for (unsigned int i = 0; i < matrix.n_rows; i++) {
		 if (matrix(i, matrix.n_cols-1) == 1){
			covPosMat.insert_rows(insertion++, matrix.submat(i, 0, i, matrix.n_cols - 2));		
		 }
	}
	return covPosMat;
}

arma::mat negMat(arma::mat &matrix) {
    covNegMat = arma::mat();
	int insertions = 0;
    for (unsigned int i = 0; i < matrix.n_rows; i++) {
        if (matrix(i, matrix.n_cols - 1) == -1)
            covNegMat.insert_rows(insertions++, matrix.submat(i, 0, i, matrix.n_cols - 2));
    }

    return covNegMat;
}

//This should be one function:
arma::mat computeCovPosMat(arma::mat &posMatrix) {
    covPosMat = cov(posMatrix);
    return covPosMat;
}

arma::mat computeCovNegMat(arma::mat &negMatrix) {
    covNegMat = cov(negMatrix);
    return covNegMat;
}

arma::mat computeTransMat(arma::mat &covPosMat,
        arma::mat &covNegMat) {
			covPosMat.print();
			covNegMat.print();
			cout<<"Trans============"<<endl;
    arma::mat transMat = covPosMat + covNegMat;
    return transMat;
}

arma::mat mappingPos(arma::mat &transMatrix,
        arma::mat &posMatrix) {
    arma::mat tmpMatrix;
    arma::mat preprocessorPos;

    arma::mat diagonalMatrix;
	
    arma::mat preprocessorNeg;
    //Need to set correct alfa
	float alfa=1.0;

    if (transMatrix.is_square()) {
        mat onesMat = arma::mat(transMatrix.n_cols, transMatrix.n_cols,
                arma::fill::ones);
		diagonalMatrix = diagmat(onesMat);
        tmpMatrix = inv(transMatrix + alfa*diagonalMatrix);
		tmpMatrix.print();
		cout<<"Inv"<<endl;
		tmpMatrix = sqrtMat(tmpMatrix);
		tmpMatrix.print();
		cout<<"Sqrt"<<endl;
		posMatrix.print();
		cout<<"Pos"<<endl;
        preprocessorPos = tmpMatrix * posMatrix.t();
    }

    return preprocessorPos;
}

arma::mat mappingNeg(arma::mat &transMatrix,
        arma::mat &negMatrix) {
    arma::mat tmpMatrix;
    arma::mat diagonalMatrix;
	
    arma::mat preprocessorNeg;
    //Need to set correct alfa
	float alfa=1.0;

    if (transMatrix.is_square()) {
		 mat onesMat = arma::mat(transMatrix.n_cols, transMatrix.n_cols,
                arma::fill::ones);
		diagonalMatrix = diagmat(onesMat);
		diagonalMatrix = transMatrix + alfa*diagonalMatrix; 
        tmpMatrix = inv(transMatrix);
        tmpMatrix = sqrtMat(tmpMatrix);
        preprocessorNeg = tmpMatrix * negMatrix.t();
    }

    return preprocessorNeg;
}

arma::mat sqrtMat(arma::mat &matrix) {
    arma::cx_vec eigenValueVec;
    arma::cx_mat eigenVectorMat;
    arma::cx_mat diagonalMat;
    arma::cx_mat finalMat;

    // Decomposes eigen values and vectors
    bool decompositionStatus = eig_gen(eigenValueVec, eigenVectorMat, matrix);
    if (!decompositionStatus) {
        throw std::runtime_error("Unable to decompose eigenvectors.");
    }

    // Computes D = V^(-1) * A * V
    diagonalMat = inv(eigenVectorMat) * matrix * eigenVectorMat;

    // Computes D^(1/2)
    int nRows = matrix.n_rows;
    int nCols = matrix.n_cols;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            if (i != j) {
                diagonalMat(i, j) = 0;
            } else {
                diagonalMat(i, j) = sqrt(diagonalMat(i, j));
            }
        }
    }

    // Computes B = V * D^(1/2) * V^(-1)
    finalMat = eigenVectorMat * diagonalMat * inv(eigenVectorMat);

    return arma::conv_to<arma::mat>::from(finalMat);
}

//TwoeSVMPostprocessor functions
//b varible is given by learned SVM
arma::rowvec projectingDataPos(arma::mat &matrix,arma::rowvec &weights){
   return weights.t()*matrix;
 }
 
arma::rowvec projectingDataNeg(arma::mat &matrix,arma::rowvec &weights){
   return weights.t()*matrix;
 }
 
 float shiftingBoundary(arma::rowvec &P1,arma::rowvec &P2){
   return b-( stddev(P1)-stddev(P2)/stddev(P1)+stddev(P2) )  
 }