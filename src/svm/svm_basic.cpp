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

void SVMConfiguration::setClassWeights(arma::vec class_weights){
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

    if(this->class_weight_length != 2){
        printf("SVMLight doesn't support multiclass classification. Please pass two class weights. \n");
        exit(1);
    }

    libsvm_class_weights[0] = class_weights(0);
    libsvm_class_weights_labels[0] = -1;
    libsvm_class_weights[1] = class_weights(1);
    libsvm_class_weights_labels[1] = 1;
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
    arma::uvec rowptr,
    arma::uvec colind,
    arma::vec values,
    size_t nrow,
    size_t ncol,
    bool one_indexed
) {
    // rowind and colptr are one-indexed -- we are sad
    if (one_indexed) {
        for (size_t i=0; i < rowptr.size(); ++i) {
            rowptr[i] -= 1;
        }
        for (size_t i=0; i < colind.size(); ++i) {
            colind[i] -= 1;
        }
    }

    // THIS IS TRICKY:
    // we are using fact that CSR format for A is CSC format for A^T
    this->sparse_data = arma::sp_mat(colind, rowptr, values, ncol, nrow);
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

void SVMConfiguration::setSeed(int seed){
	srand(seed);
	this->seed = seed;
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
    

    use_transductive_learning = false;
    transductive_posratio = -1;

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

    max_iter = 10000000;
}

