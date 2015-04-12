#include "svm_basic.h"
#include "svm.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration() {
	this->prediction = false;
	SVMConfiguration::setDefaultParams();
}

int SVMConfiguration::getDataExamplesNumber() {
	if(isSparse()) {
        // FIXME: change to sparse_data.n_rows after libsvm refactor
		return this->dim;
	} else {
		return this->data.n_rows;
	}
}

int SVMConfiguration::getDataDim() {
	if(isSparse()) {
        // FIXME: change to sparse_data.n_cols after libsvm refactor
		return this->data_dim;
	} else {
		return this->data.n_cols;
	}
}

size_t SVMConfiguration::getSVCount() {
    return this->support_vectors.n_rows;
}

SVMConfiguration::SVMConfiguration(bool prediction) {
	this->prediction = prediction;
}

void SVMConfiguration::setSparse(bool sparse) {
	this->sparse = sparse;
}

void SVMConfiguration::setSparseData(
	arma::uvec rowind,
    arma::uvec colptr,
    arma::vec values,
    size_t nrow,
    size_t ncol
) {
    //this->sparse_data = arma::sp_mat(rowind, colptr, values, n_rows, n_cols);
    this->sparse_data = arma::sp_mat(nrow, ncol);

    // create space for values, and copy
    arma::access::rw(this->sparse_data.values)
        = arma::memory::acquire_chunked<double>(values.size() + 1);
    arma::arrayops::copy(
        arma::access::rwp(this->sparse_data.values), values.begin(), values.size() + 1);

    // create space for row_indices, and copy
    arma::access::rw(this->sparse_data.row_indices)
        = arma::memory::acquire_chunked<arma::uword>(rowind.size() + 1);
    arma::arrayops::copy(
        arma::access::rwp(this->sparse_data.row_indices), rowind.begin(), rowind.size() + 1);
    
    // create space for col_ptrs, and copy 
    arma::access::rw(this->sparse_data.col_ptrs)
        = arma::memory::acquire<arma::uword>(colptr.size() + 2);
    arma::arrayops::copy(
        arma::access::rwp(this->sparse_data.col_ptrs), colptr.begin(), colptr.size() + 1);

    // important: set the sentinel as well
    arma::access::rwp(this->sparse_data.col_ptrs)[colptr.size()+1]
        = std::numeric_limits<arma::uword>::max();
    
    // set the number of non-zero elements
    arma::access::rw(this->sparse_data.n_nonzero) = values.size();
}

arma::sp_mat SVMConfiguration::getSparseData() {
    return this->sparse_data;
}

bool SVMConfiguration::isSparse() {
	return this->sparse;
}

void SVMConfiguration::setFilename(std::string filename) {
	this->filename = filename;
}

std::string SVMConfiguration::getFilename() {
	return this->filename;
}

void SVMConfiguration::setModelFilename(std::string filename) {
	this->model_filename = filename;
}

std::string SVMConfiguration::getModelFilename() {
	return this->model_filename;
}

void SVMConfiguration::setData(arma::mat data) {
	this->data = data;
}

arma::mat SVMConfiguration::getData() {
	return this->data;
}

void SVMConfiguration::setOutputFilename(std::string filename) {
	this->output_filename = filename;
}
std::string SVMConfiguration::getOutputFilename() {
	return this->output_filename;
}

bool SVMConfiguration::isPrediction() {
	return this->prediction;
}

void SVMConfiguration::setPrediction(bool prediction) {
	this->prediction = prediction;
}

#ifdef RCPP_INTERFACE
void SVMConfiguration::setWeights( Rcpp::NumericVector weights ) {
	this->nr_weight = weights.size();
	this->weight = new double[nr_weight];

	for (int i = 0; i < nr_weight; i++) {
		weight[i] = weights(1);
	}
}
#endif

void SVMConfiguration::setLibrary( std::string library ) {
	if ( library == "libsvm" ) {
		this->library = LIBSVM;
		this->svm_type = C_SVC;
	}
	else if (library == "svmlight" ) {
    this->library = SVMLIGHT;
    this->svm_type = C_SVC;
	}
}

void SVMConfiguration::setKernel( std::string kernel ) {
	if ( kernel == "linear" ) {
		this->kernel_type = _LINEAR;
	}
	else if ( kernel == "poly" ) {
		this->kernel_type = _POLY;
	}
	else if ( kernel == "rbf" ) {
		this->kernel_type = _RBF;
	}
	else if ( kernel == "sigmoid" ) {
		this->kernel_type = _SIGMOID;
	}
}

void SVMConfiguration::setPreprocess( std::string preprocess ) {
	if ( preprocess ==  "2e" ) {
		this->preprocess = TWOE;
	}
	else if ( preprocess == "none" ) {
		this->preprocess = NONE;
	}
}

void SVMConfiguration::set_verbosity(int verbosity){
  this->log.verbosity = verbosity;
}

double SVMConfiguration::getB() {
  return threshold_b;
}

void SVMConfiguration::setB(double b) {
  threshold_b = b;
}

void SVMConfiguration::setDefaultParams() {
	library = LIBSVM;
	svm_type = C_SVC;
	kernel_type = _LINEAR;
  preprocess = NONE;
	degree = 3;
	gamma = 0;	// 1/num_features
	coef0 = 0;
	cache_size = 100;
	C = 1;
	eps = 1e-3;
	shrinking = 1;
	probability = 0;
	nr_weight = 0;
	weight_label = NULL;
	weight = NULL;
	cov_eps_smoothing_start = 2.22e-16; //2^(-23)
	//cov_eps_smoothing_start = 
//	Probably not necessery
	nu = 0.5;
	p = 0.1;
    
    // Sparse data
    sparse = false;

    // Additional features
    use_cost = false;

    // User-defined classification mode labels
    // (will be filled during data processing)
    neg_target = 0;
    pos_target = 0;
}

