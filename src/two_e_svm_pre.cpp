#include <cmath>
#include <RcppArmadillo.h>
#include "two_e_svm_pre.h"

#include "svm_handler.h"
#include "svm_basic.h"

using namespace arma;
void TwoeSVMPreprocessor::processRequest(SVMConfiguration& data) {
	if (data.prediction) {
		arma::mat workingMat = data.data;

		arma::mat pos = cov(posMat(workingMat));
		arma::mat neg = cov(negMat(workingMat));

		arma::mat mapPos = mappingPos(computeTransMat(pos, neg),
				posMat(workingMat));
		arma::mat mapNeg = mappingNeg(computeTransMat(pos, neg),
				negMat(workingMat));

		//Union on pos and neg matrix
		mapPos.insert_rows(0, mapNeg);
		data.setData(mapPos);
	}
}

bool TwoeSVMPreprocessor::canHandle(SVMConfiguration& data) {
	// TODO
	return true;
}

arma::mat TwoeSVMPreprocessor::posMat(arma::mat &matrix) {
	arma::mat covPosMat;
	int insertion = 0;
	for (unsigned int i = 0; i < matrix.n_rows; i++) {
		if (matrix(i, matrix.n_cols - 1) == 1) {
			covPosMat.insert_rows(insertion++,
					matrix.submat(i, 0, i, matrix.n_cols - 1));
		}
	}
	return covPosMat;
}

arma::mat TwoeSVMPreprocessor::negMat(arma::mat &matrix) {
	covNegMat = arma::mat();
	int insertions = 0;
	for (unsigned int i = 0; i < matrix.n_rows; i++) {
		if (matrix(i, matrix.n_cols - 1) == -1)
			covNegMat.insert_rows(insertions++,
					matrix.submat(i, 0, i, matrix.n_cols - 1));
	}

	return covNegMat;
}

arma::mat TwoeSVMPreprocessor::computeTransMat(arma::mat &covPosMat,
		arma::mat &covNegMat) {
	transMat = covPosMat + covNegMat;
	return transMat;
}

arma::mat TwoeSVMPreprocessor::mappingPos(arma::mat transMatrix,
		arma::mat posMatrix) {
	arma::mat tmpMatrix;
	arma::mat preprocessorPos;
	arma::mat diagonalMatrix;
	arma::mat preprocessorNeg;
	//Need to set correct alfa
	float alfa = 1.0;

	if (transMatrix.is_square()) {
		mat onesMat = arma::mat(transMatrix.n_cols, transMatrix.n_cols,
				arma::fill::ones);
		diagonalMatrix = diagmat(onesMat);
		tmpMatrix = inv(transMatrix + alfa * diagonalMatrix);
		tmpMatrix = sqrtMat(tmpMatrix);
		preprocessorPos = tmpMatrix * posMatrix.t();
	}

	return preprocessorPos.t();

}

arma::mat TwoeSVMPreprocessor::mappingNeg(arma::mat transMatrix,
		arma::mat negMatrix) {

	arma::mat tmpMatrix;
	arma::mat diagonalMatrix;

	arma::mat preprocessorNeg;
	//Need to set correct alfa
	float alfa = 1.0;

	if (transMatrix.is_square()) {
		mat onesMat = arma::mat(transMatrix.n_cols, transMatrix.n_cols,
				arma::fill::ones);
		diagonalMatrix = diagmat(onesMat);
		diagonalMatrix = transMatrix + alfa * diagonalMatrix;
		tmpMatrix = inv(transMatrix);
		tmpMatrix = sqrtMat(tmpMatrix);
		preprocessorNeg = tmpMatrix * negMatrix.t();
	}

	return preprocessorNeg.t();

}

arma::mat TwoeSVMPreprocessor::sqrtMat(arma::mat &matrix) {
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
