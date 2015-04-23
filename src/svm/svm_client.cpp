#include <math.h>

#include "svm_client.h"
#include "libsvm_runner.h"
#include "svmlight_runner.h"
#include "two_e_svm_pre.h"
#include "two_e_svm_post.h"
#include "svm_utils.h"


const std::string __file__ = "svm_client.cpp";
const std::string __client_class__ = "SVMClient";
const std::string __debug_prefix__ = __file__ + "." + __client_class__;


// Constructor
SVMClient::SVMClient(SVMConfiguration *config): config(*config) {}

// Setters
void SVMClient::setX( arma::mat x ){
    config.data = x;
}
void SVMClient::setY( arma::vec y ){
    config.target = y;
}

void SVMClient::setLibrary(std::string library){
    config.setLibrary(library);
}
void SVMClient::setKernel(std::string kernel){
    config.setKernel(kernel);
}
void SVMClient::setPreprocess(std::string prep){
    config.setPreprocess(prep);
}

void SVMClient::setCacheSize(double cache) {
    config.cache_size = cache;
}
void SVMClient::setDegree(int degree){
    config.degree = degree;
}
void SVMClient::setGamma(double gamma){
    config.gamma = gamma;
}
void SVMClient::setCoef0(double coef0 ){
    config.coef0 = coef0;
}
void SVMClient::setC(double C){
    config.C = C;
}
void SVMClient::setEps(double eps){
    config.eps = eps;
}
void SVMClient::setShrinking(int sh){
    config.shrinking = sh;
}
void SVMClient::setProbability(int prob){
    config.probability = prob;
}

void SVMClient::setConfiguration(SVMConfiguration *config) {
    SVMConfiguration current_config = *config;
    this->config = current_config;
}

void SVMClient::setW(arma::vec new_w){
    if (config.kernel_type != _LINEAR) {
        LOG(config.log, LogLevel::ERR, "ERROR: " + to_string("Decision boundary is not available with non-linear kernel"));
    }
    else if (new_w.n_elem != config.w.n_elem) {
        LOG(config.log, LogLevel::ERR, "ERROR: " + to_string("Vectors are of different length"));
    }
    else 
        config.w = new_w;
}

void  SVMClient::setAlpha(arma::vec new_alpha){
    if (new_alpha.n_elem != config.alpha_y.n_elem){
        LOG(config.log, LogLevel::ERR, "ERROR: " + to_string("Vectors are of different length"));
    }
    else {
        config.alpha_y = new_alpha;
    }
}

// Getters
arma::mat SVMClient::getX(){
    return config.data;
}
arma::vec SVMClient::getY(){
    return config.target;
}
arma::vec SVMClient::getPrediction() {
    return SVMClient::config.result;
}
std::string SVMClient::getLibrary(){
    switch(config.svm_type) {
    case LIBSVM : return "libsvm";
    case SVMLIGHT : return "svmlight";
    default : return "error"; 
    }
}
std::string SVMClient::getKernel(){
    switch(config.kernel_type) {
    case _LINEAR : return "linear";
    case _POLY : return "poly"; 
    case _RBF : return "rbf"; 
    case _SIGMOID : return "sigmoid"; 
  default : return "error"; 
    }
}
std::string SVMClient::getPreprocess() {
    switch(config.preprocess) {
    case TWOE : return "2e";
    case NONE : return "none";
  default : return "error";
    }
}
double SVMClient::getCacheSize(){
    return config.cache_size;
}
int SVMClient::getDegree(){
    return config.degree;
}
double SVMClient::getGamma(){
    return config.gamma;
}
double SVMClient::getCoef0(){
    return config.coef0;
}
double SVMClient::getC(){
    return config.C;
}
double SVMClient::getEps(){
    return config.eps;
}
bool SVMClient::isShrinking(){
    return (bool)config.shrinking;
}
bool SVMClient::isProbability(){
    return (bool)config.probability;
}

bool SVMClient::isSparse() {
    return (bool)config.isSparse();
}

arma::vec SVMClient::getExampleWeights(){
    return config.example_weights;
}

arma::vec SVMClient::getClassWeights(){
    return config.class_weights;
}



bool 
SVMClient::areExamplesWeighted() {
    return (bool)config.use_example_weights;
}
// model getters
arma::vec SVMClient::getAlpha() {
    return config.alpha_y;
}

//void SVMClient::setAlpha(double* alpha) {
//  if (config. - length(alpha) == 1) {
//    for (int i = 0; i != length(alpha); i++) {
//      config.rho[i+1] = alpha[i];
//    }
//  } 
//  else if (length(config.rho) != length(alpha)) {
//    LOG(config.log, logLevel::ERR, "ERROR: " + to_string("Wrong alpha array size."));
//    return;
//  }
//  else {
//    config.rho = alpha;
//  }
//}

void SVMClient::setBias(double bias) {
    config.setB(bias);
}

double SVMClient::getBias() {   
    return config.getB();       
}

arma::vec SVMClient::getW() {       
    if ( config.kernel_type == _LINEAR ) {
        return config.w;
    }
    else {
    	LOG(config.log, LogLevel::ERR, "ERROR: " + to_string("Decision boundary is not available with non-linear kernel"));
        return 0;
    }
}

int SVMClient::get_number_sv() {
  return config.support_vectors.n_rows;
}

int SVMClient::get_number_class() {
  return config.nr_class;
}

arma::sp_mat SVMClient::getSV(){
  return config.support_vectors;
}

SVMConfiguration &SVMClient::getConfiguration() {
    return this->config;
}

// Runners
void SVMClient::run() {
    SVMClient::createFlow();
    for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
            iter != SVMHandlers.end(); ++iter) {
        (*iter)->processRequest(config);
    }
}

void SVMClient::train() {
    config.setPrediction(false);
    run();
}

void SVMClient::predict( arma::mat problem ) {
    
    // Just like in requestPredict()
    config.setData(problem);

    predictFromConfig();

}

void SVMClient::predictFromConfig() {
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".predictFromConfig() Started.");

    // FIXME: Calculate TwoE prediction in this method
    if (config.preprocess == TWOE) {
        // Request prediction from handlers when not globally supported
        requestPredict();
        return;
    }

    // Number of docs is a number of rows in data matrix
    size_t n_docs = config.getDataExamplesNumber();
    config.result = arma::zeros<arma::vec>(n_docs);


    LOG(config.log, LogLevel::DEBUG,
        __debug_prefix__ + ".predictFromConfig() Calculating prediction on " +
		svm_to_str(n_docs) + " documents...");

    LOG(config.log, LogLevel::TRACE,
        __debug_prefix__ + ".predictFromConfig() Calculating prediction onto "
        		+svm_to_str(config.pos_target) + " "+svm_to_str(config.neg_target)+
        		"..");

    if(config.kernel_type == ::_LINEAR){
        LOG(config.log, LogLevel::TRACE,
            __debug_prefix__ + ".predictFromConfig() using linear prediction");
    }

    // Prediction itself
    // math:
    // f(x) = sum{alpha_j * y_j * kernel(x_j, x)} + b, where j means j-th SV}
    for (int i=0; i < n_docs; ++i) {
    	double doc_result = 0;
        // For linear kernel it is possible to speed up
        if(config.kernel_type == ::_LINEAR){
            if (isSparse()) {
            	doc_result = arma::dot(
                    config.w.t(),
                    config.getSparseData().col(i).t()
                );
            } else {
            	doc_result = arma::dot(
                    config.data.row(i),
                    config.w.t()
                );
            }
        } else {
        	for (int j=0; j < config.getSVCount(); ++j) {
        		doc_result += kernel(i, j) * config.alpha_y(j);
        	}
        }
        config.result[i] = doc_result + config.b;
//        LOG(config.log, 5, "Decision function "+svm_to_str(doc_result));
    }

    if(n_docs){
		LOG(config.log, LogLevel::DEBUG,
			__debug_prefix__ + ".predictFromConfig() Coverting to labels, first"
					"result is " + svm_to_str(config.result[0])+
					"...");

    }
    // Convert results to userdefined labels
    n_docs = config.result.n_rows;
    double doc_result = 0;
    for (int i=0; i < n_docs; ++i) {
        doc_result = config.result[i];

        // Store user-defined label
        if (doc_result > 0) {
            config.result[i] = config.pos_target;
        } else {
            config.result[i] = config.neg_target;
        }
    }

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".predictFromConfig() Done."
    );
}

double SVMClient::kernel(size_t i, size_t j) {
    double result=0.0;
    switch (config.kernel_type) {
        case _LINEAR: {
            // math:     kernel(x, x') = x^T * x'
            // libsvm:   kernel(v, u') = u'*v
            // svmlight: kernel(b, a) = a*b
            if (isSparse()) {
                result = arma::dot(
                    config.getSparseData().col(i),
                    config.support_vectors.row(j).t()
                );
            } else {
                result = arma::dot(
                    config.data.row(i),
                    config.support_vectors.row(j)
                );
            }
            break;
        }
        case _POLY: {
            // libsvm:   kernel(v, u') = (gamma*u'*v + coef0)^degree
            // svmlight: kernel(b, a) = (s a*b+c)^d
            if (isSparse()) {
                result = arma::dot(
                    config.getSparseData().col(i),
                    config.support_vectors.row(j).t()
                );
            } else {
                result = arma::dot(
                    config.data.row(i),
                    config.support_vectors.row(j)
                );
            }

            return pow(result * config.gamma + config.coef0, config.degree);
        }
        case _RBF: {
            // libsvm:   kernel(v, u') = exp(-gamma*|u-v|^2)
            // svmlight: kernel(b, a) = exp(-gamma ||a-b||^2)
            double neg_gamma = -config.gamma;
            double norm = 0;
            if (isSparse()) {
                norm = arma::norm(
                    config.getSparseData().col(i).t()
                    - config.support_vectors.row(j),
                    2
                );
            } else {
                norm = arma::norm(
                    config.data.row(i)
                    - config.support_vectors.row(j),
                    2
                );
            }
            return exp(neg_gamma * norm * norm);
        }
        case _SIGMOID: {
            // libsvm:   kernel(v, u') = tanh(gamma*u'*v + coef0)
            // svmlight: kernel(b, a) = tanh(s a*b + c)
            double tanh_arg = 0;
            if (isSparse()) {
                tanh_arg = arma::dot(
                    config.getSparseData().col(i),
                    config.support_vectors.row(j).t()
                );
            } else {
                tanh_arg = arma::dot(
                    config.data.row(i),
                    config.support_vectors.row(j)
                );
            }
            return tanh(tanh_arg * config.gamma + config.coef0);
        }
        default: {
            LOG(
                config.log,
                LogLevel::FATAL,
                __debug_prefix__ + ".kernel() invalid kernel type!"
            );
            break;
        }
    }
    return result;
}

void SVMClient::sparse_predict(
    arma::uvec rowind,
    arma::uvec colptr,
    arma::vec values,
    size_t n_rows,
    size_t n_cols
) {
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".sparse_predict() Started.");

    config.setSparseData(
        rowind,
        colptr,
        values,
        n_rows,
        n_cols,
        true
    );

    if (config.debug_library_predict) {
        config.setPrediction(true);
        requestPredict();
    } else {
        predictFromConfig();
    }

    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".sparse_predict() Done.");
}

void SVMClient::requestPredict() {
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".requestPredict() Started.");
    if ( SVMHandlers.size() > 0 ) {
        config.setPrediction(true);
        for (std::vector<SVMHandler*>::iterator iter = SVMHandlers.begin();
                iter != SVMHandlers.end(); ++iter) {
            (*iter)->processRequest(config);
        }
    }
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".requestPredict() Done.");
}

void SVMClient::createFlow() {
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".createFlow() Started.");
    SVMType svm_type = config.library;
    Preprocess preprocess = config.preprocess;
    std::vector<SVMHandler*> handlers;


        switch (svm_type) {
            case LIBSVM: {
                LibSVMRunner *runner = new LibSVMRunner();
                handlers.push_back(runner);
                break;
            }
            case SVMLIGHT : {   
        SVMLightRunner *runner = new SVMLightRunner();      // Wating for svm light runner implementation
                handlers.push_back(runner);
                break;
            }
            default: {
                LibSVMRunner *runner = new LibSVMRunner();              // dafault will be libsvm
                handlers.push_back(runner);
                break;
            }
        }

    switch (preprocess) {
      case TWOE :   {   
          TwoeSVMPostprocessor *post_runner = new TwoeSVMPostprocessor();
                    TwoeSVMPreprocessor *pre_runner = new TwoeSVMPreprocessor();
                    handlers.insert( handlers.begin(), pre_runner );
                    handlers.push_back( post_runner );
                    break;
      }
    case NONE:
        break; 
    default:
        break;
    }

    this->SVMHandlers = handlers;
    LOG(config.log, LogLevel::DEBUG, __debug_prefix__ + ".createFlow() Done.");
}

