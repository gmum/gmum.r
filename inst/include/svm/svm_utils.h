#ifndef SVC_UTILS_H_
#define SVC_UTILS_H_
#include <svm.h>
//#include <RcppArmadillo.h>
#include <armadillo>

#ifndef DEBUG
#define ASSERT(x)
#else
#define ASSERT(x) \
 if (! (x)) \
 { \
    cout << "ERROR!! Assert " << #x << " failed\n"; \
    cout << " on line " << __LINE__  << "\n"; \
    cout << " in file " << __FILE__ << "\n";  \
 }
#endif


using namespace std;

class SvmUtils {
private:
  SvmUtils();
	virtual ~SvmUtils();
public:

	static void sqrtInvMat(const arma::mat &matrix, arma::mat &finalMat) {
		arma::vec eigenValue;
		arma::mat eigenVector;
		arma::mat diagonalMat;
		// Throws runtime decomposition error
		arma::mat inverse = arma::inv(matrix);
		arma::eig_sym(eigenValue, eigenVector,inverse);
		finalMat = eigenVector * arma::sqrt(arma::diagmat(eigenValue))	* eigenVector.t();
	}

	//convert sparse matrix to armadillo matrix
	static arma::mat libtoarma(svm_node** svm_nodes, int nr_sv, int dim) {
		arma::mat ret(nr_sv, dim);
		
		for (int row = 0; row < nr_sv; row++) {
			svm_node* tmp_row = svm_nodes[row];
			for (int j = 0; tmp_row[j].index != -1; j++) {
				// cout << "Row, j, index, value" << row << " " << j << " " <<
				// tmp_row[j].index << " " << tmp_row[j].value << endl;
				ret(row, tmp_row[j].index - 1) = tmp_row[j].value;
			}
		}
		return ret;
	}

	//TODO: resize ret matrix
	static void libToArma(svm_node** svm_nodes, int nr_sv, int dim, arma::mat ret) {
		//TODO: resize ret matrix
		// arma::mat ret(nr_sv, dim);

		for (int row = 0; row < nr_sv; row++) {
			
			svm_node* tmp_row = svm_nodes[row];
			for (int j = 0; tmp_row[j].index != -1; j++) {
				// cout << "Row, j, index, value" << row << " " << j << " " <<
				// tmp_row[j].index << " " << tmp_row[j].value << endl;
				ret(row, tmp_row[j].index - 1) = tmp_row[j].value;
			}
		}
	}

	static arma::vec arrtoarmavec(double* arr, int size) {
		arma::vec ret(size);
		for (int i = 0; i < size; i++) {
			ret(i) = arr[i];
		}
		return ret;
	}

//	TODO: MAKE IT WORKING FOR MULTICLASS
	static arma::vec arrtoarmavec(double** arr, int size) {
		return arrtoarmavec(arr[0], size);
	}

	static arma::mat matrixByValue(arma::mat &data, arma::vec &targets,
			double value) {
		return data.rows(find(targets == value));
	}

};

template <typename T>
static std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}

#endif /* SVC_UTILS_H_ */
