#include "svm_basic.h"
#include "svm.h"

// SVM Configuration 
// Constructors  
SVMConfiguration::SVMConfiguration(): label(0), nSV(0),  libsvm_class_weights(0) {
    this->prediction = false;
    SVMConfiguration::setDefaultParams();
}

int SVMConfiguration::getDataExamplesNumber() {
    if(isSparse()) {
        return this->sparse_data.n_cols;
    } else {
        return this->data.n_rows;
    }
}

void SVMConfiguration::setClassWeights(arma::vec class_weights, arma::vec class_weights_labels){
	this->class_weights = class_weights;
    this->use_class_weights = true;
	class_weight_length = class_weights.size();

    if(libsvm_class_weights){
    	delete[] libsvm_class_weights;
    }
    if(libsvm_class_weights_labels){
    	delete[] libsvm_class_weights_labels;
    }
    libsvm_class_weights = new double[class_weight_length];
    libsvm_class_weights_labels = new int[class_weight_length];
    for(size_t i=0; i<class_weight_length; ++i){
    	libsvm_class_weights[i] = class_weights(i);
    	libsvm_class_weights_labels[i] = (int)class_weights_labels(i);
    }
}

int SVMConfiguration::getDataDim() {
    if(isSparse()) {
        return this->sparse_data.n_rows;
    } else {
        return this->data.n_cols;
    }
}

size_t SVMConfiguration::getSVCount() {
    return this->support_vectors.n_cols;
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
    size_t ncol,
    bool one_indexed
) {
    // rowind and colptr are one-indexed -- we are sad
    if (one_indexed) {
        for (size_t i=0; i < rowind.size(); ++i) {
            rowind[i] -= 1;
        }
        for (size_t i=0; i < colptr.size(); ++i) {
            colptr[i] -= 1;
        }
    }

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

    this->sparse_data = this->sparse_data.t();

}

arma::sp_mat &SVMConfiguration::getSparseData() {
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
  return b;
}

void SVMConfiguration::setB(double b) {
  this->b = b;
}

void SVMConfiguration::setDefaultParams() {
    library = LIBSVM;
    svm_type = C_SVC;
    kernel_type = _LINEAR;
    preprocess = NONE;
    degree = 3;
    gamma = 0;  // 1/num_features
    coef0 = 0;
    cache_size = 100;
    C = 1;
    eps = 1e-3;
    shrinking = 1;
    probability = 0;
    class_weight_length = 0;
    libsvm_class_weights_labels = NULL;
    libsvm_class_weights = NULL;
    cov_eps_smoothing_start = 2.22e-16; //2^(-23)
    //cov_eps_smoothing_start = 

    //TODO: Probably not necessery
    nu = 0.5;
    p = 0.1;
    
    // Sparse data
    sparse = false;

    // Additional features
    use_example_weights = false;
    use_class_weights = false;

    // User-defined classification mode labels
    // (will be filled during data processing)
    neg_target = 0;
    pos_target = 0;

    debug_library_predict = false;
}

