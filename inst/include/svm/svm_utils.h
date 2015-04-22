#ifndef SVC_UTILS_H_
#define SVC_UTILS_H_
#include <svm.h>
#include <armadillo>
#include <sstream>

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

//TODO: clean after consolidating svm_utils.h
//and log.h with utils/log.hpp and utils/utils.h
template<class T>
std::string svm_to_str(const T& x) {
	stringstream ss;
	ss << x;
	return ss.str();
}

class SvmUtils {
private:
  SvmUtils();
	virtual ~SvmUtils();
public:

	static double sqrtInvMat(arma::mat &matrix, arma::mat &finalMat, double cov_eps_smoothing_start = 0) {
		arma::vec eigenValue;
		arma::mat eigenVector;
		arma::mat diagonalMat;
		arma::mat inverse;
		double mu = arma::trace(matrix) / matrix.n_rows;
		double cov_eps_smoothing_end = cov_eps_smoothing_start;
		bool not_singular = false;
		while(!not_singular) {
			not_singular = inv_sympd(inverse,matrix);
			matrix = (1-cov_eps_smoothing_end) * matrix +
			mu * cov_eps_smoothing_end * arma::eye(matrix.n_cols, matrix.n_cols);
			cov_eps_smoothing_end *= 2;
		}
		arma::eig_sym(eigenValue, eigenVector,inverse);
		finalMat = eigenVector * arma::sqrt(arma::diagmat(eigenValue))	* eigenVector.t();
		return cov_eps_smoothing_end;
	}

	//convert sparse matrix to armadillo matrix
	static arma::mat libtoarma(svm_node** svm_nodes, int nr_sv, int dim) {
		arma::mat ret(nr_sv, dim, arma::fill::zeros);
		
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
	static void libToArma(svm_node** svm_nodes, int nr_sv, int dim, arma::mat& ret) {
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

	//static void ArmaVec2

};

template <typename T>
static std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}

#endif /* SVC_UTILS_H_ */
