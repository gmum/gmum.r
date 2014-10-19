#' @title SVM
#' 
#' @export
#' 
#' @description Create SVM model object. If any parameter will be omitted a default value will be used
#' 
#' @param x Dataset without labels
#' @param y Labels
#' @param lib Desired SVM Library, avialable are: libsvm
#' @param kernel Kernel type, avialable are: linear, poly, rbf, sigmoid
#' @param prep Preprocess method, avialable are: none, 2e
#' @param mclass Multiclass wariant, avialable are: none
#' @param C Cost/Complexity parameter
#' @param gamma Gamma parameter for poly, rbf and sigmoid kernels
#' @param coef0 Coef0 for poly and sigmoid kernels
#' @param degree Degree for poly kernel
#' @param shrinking Whether to use shrinking heuristics
#' @param probability Whether to train a model for probability estimates 
#' @param cache_size Cache size
#' @param tol Tolerance of termination criterion
#' 
#' @return SVM model object
#' @examples 
#' svm <- SVM(lib = "libsvm", kernel = "linear", C = 1, gamma = 0.01, coef0 = 0,degree = 3)
#' 
SVM <- NULL

#' @title Train SVM model
#' 
#' @description Trains provided svm object, using its parameters and dataset.
#' 
#' @rdname svm-train-methods
#' 
#' @export
#' 
#' @usage train(svm)
#' 
#' @param object Trained SVM object which will be used for prediction.
#' 
#' @docType methods
#' 
#' @aliases train
train.svm <- NULL

#' @title Predict
#' 
#' @description Returns predicted classes for provided test examples, note that provided model needs to be trained first before any prediction can be made.
#' 
#' @rdname svm-predict-methods
#' 
#' @export
#' 
#' @usage predict(svm, x)
#' 
#' @param object Trained SVM object.
#' @param x unlabeled data, note that each entry needs to be the same dimentionality as training examples.
#' 
#' @docType methods
#' 
#' @aliases test
predict.svm <- NULL

#' @title print
#' 
#' @description Prints short summary of the SVM object and its parameters.
#' 
#' @rdname svm-print-methods
#' 
#' @export
#' 
#' @usage print(svm)
#' 
#' @param object SVM object
#' 
#' @docType methods
#' @aliases summary
print.svm <- NULL

#' @title plot
#' 
#' @description Draws choosen dimenstions from a dataset on 2D plot. NOTE: This function will be change to a package default function.
#' 
#' @export
#' 
#' @usage plot(svm)
#' 
#' @rdname plot-dataset-methods
#' 
#' @docType plot
plot.svm <- NULL

#' @title load.dataset
#' 
#' @description Loads provided dataset to a SVM object, right now lables need to be in the last column.
#' 
#' @rdname svm-load-methods
#' 
#' @export
#' 
#' @usage load.dataset(svm, x)
#' 
#' @param object SVM object to which examples will be loaded.
#' @param x Labeled dataset with lables in the last column.
#' 
#' @docType methods
#' 
#' @aliases dataset
load.dataset <- NULL

#' @title dataset.X
#' 
#' @description Prints dataset stored in a SVM object, without the labels.
#' 
#' @rdname print-x-methods
#' 
#' @export
#' 
#' @usage dataset.X(svm)
#' 
#' @param object SVM object.
#' 
#' @docType methods
#' 
#' @aliases dataset
dataset.X <- NULL

#' @title dataset.Y
#' 
#' @description Prints lables stored in a SVM object.
#' 
#' @rdname print-y-methods
#' 
#' @export
#' 
#' @usage dataset.Y(svm)
#' 
#' @param object SVM object.
#' 
#' @docType methods
#' 
#' @aliases dataset
dataset.Y <- NULL


#' @title params
#' 
#' @description Changes SVM objects parameters to provided, not every parameter needs to be given.
#' 
#' @rdname svm-parameters-method
#' 
#' @export
#' 
#' @usage params(svm, object, lib, kernel, preprocess, C, gamma, coef0, degree, cache_size, shrinking, probability)
#' 
#' @param lib Desired SVM Library, avialable are: libsvm.
#' @param kernel Kernel type, avialable are: linear, poly, rbf, sigmoid.
#' @param prep Preprocess method, avialable are: none, 2e.
#' @param C Cost/Complexity parameter.
#' @param gamma Gamma parameter for poly, rbf and sigmoid kernels.
#' @param coef0 Coef0 for poly and sigmoid kernels.
#' @param degree Degree for poly kernel.
#' @param shrinking Whether to use shrinking heuristics.
#' @param probability Whether to train a model for probability estimates .
#' @param cache_size Cache size.
#' 
#' @examples params(svm, kernel="linear", preproces="none", C=10000)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm <- NULL

#' @title params.lib
#' 
#' @description Changes or prints (if a argument is not provided) library in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-lib-method
#' 
#' @export
#' 
#' @usage 
#' params.lib(svm)
#' params.lib(svm,x)
#' 
#' @param object SVM object.
#' @param x New library.
#' 
#' @examples params.lib(svm, "libsvm")
#' 
#' @docType methods
#'  
#' @aliases parameters
params.svm.lib <- NULL

#' @title params.kernel
#' 
#' @description Changes or prints (if a argument is not provided)kernel in a SVM object, note: requires retraining. Aviable are: libsvm.
#' 
#' @rdname svm-params-kernel-method
#' 
#' @export
#' 
#' @usage 
#' params.kernel(svm)
#' params.kernel(svm,x)
#' 
#' @param object SVM object.
#' @param x New kernel.
#' 
#' @examples params.kernel(svm, "rbf")
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.kernel <- NULL

#' @title params.prep
#' 
#' @description Changes or prints (if a argument is not provided) preprocess type in a SVM object, note: requires retraining. Aviable are: linear, sigmoid, poly, rbf.
#' 
#' @rdname svm-params-prep-method
#' 
#' @export
#' 
#' @usage 
#' params.prep(svm)
#' params.prep(svm,x)
#' 
#' @param object SVM object.
#' @param x New preprocess type.
#' 
#' @examples params.prep(svm, "2e")
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.prep <- NULL

#' @title params.C
#' 
#' @description Changes or prints (if a argument is not provided) Cost parameter in a SVM object, note: requires retraining. Aviable are: none, 2e
#' 
#' @rdname svm-params-C-methodd
#' 
#' @export
#' 
#' @usage 
#' params.C(svm)
#' params.C(svm,x)
#' 
#' @param object SVM object.
#' @param x New cost.
#' 
#' @examples params.C(svm, 1000)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.C <- NULL

#' @title params.gamma
#' 
#' @description Changes or prints (if a argument is not provided) gamma parameter in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-gamma-method
#' 
#' @export
#' 
#' @usage 
#' params.gamma(svm)
#' params.gamma(svm,x)
#' 
#' @param object SVM object.
#' @param x New gamma.
#' 
#' @examples params.gamma(svm, 0.01)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.gamma <- NULL

#' @title params.degree
#' 
#' @description Changes or prints (if a argument is not provided) degree in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-degree-method
#' 
#' @export
#' 
#' @usage 
#' params.degree(svm)
#' params.degree(svm,x)
#' 
#' @param object SVM object.
#' @param x New degree.
#' 
#' @examples params.degree(svm, 4)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.degree <- NULL

#' @title params.coef0
#' 
#' @description Changes or prints (if a argument is not provided) coef0 parameter in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-coef0-method
#' 
#' @export
#' 
#' @usage 
#' params.coef0(svm)
#' params.coef0(svm,x)
#' 
#' @param object SVM object.
#' @param x coef0.
#' 
#' @examples params.coef0(svm, 0)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.coef0 <- NULL

#' @title params.cache_size
#' 
#' @description Changes or prints (if a argument is not provided) cache_size (in MB) in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-cache-method
#' 
#' @export
#' 
#' @usage 
#' params.cache_size(svm)
#' params.cache_size(svm,x)
#' 
#' @param object SVM object.
#' @param x New cache size.
#' 
#' @examples params.cache_size(svm, 100)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.cache_size<- NULL

#' @title params.shrinking
#' 
#' @description Changes or prints (if a argument is not provided) if shrinking heurestics are used in a SVM object, note: requires retraining.
#' 
#' @rdname svm-params-shrin-method
#' 
#' @export
#' 
#' @usage 
#' params.shrinking(svm)
#' params.shrinking(svm,x)
#' 
#' @param object SVM object.
#' @param x TRUE/FALSE.
#' 
#' @examples params.shrinking(svm, TRUE)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.shrinking <- NULL

#' @title params.probability
#' 
#' @description Changes or prints (if a argument is not provided) if a SVM object is used for probability estimates , note: requires retraining.
#' 
#' @rdname svm-params-prob-method
#' 
#' @export
#' 
#' @usage 
#' params.probability(svm)
#' params.probability(svm,x)
#' 
#' @param object SVM object.
#' @param x TRUE/FALSE.
#' 
#' @examples params.probability(svm, TRUE)
#' 
#' @docType methods
#' 
#' @aliases parameters
params.svm.probability <- NULL

loadModule('svm_wrapper', TRUE)


evalqOnLoad({

  SVM <<- function(x = svm.breast_cancer.x(), 
                  y = svm.breast_cancer.y(),
                  lib = "libsvm",
                  kernel = "linear",
                  prep = "none",
                  mclass = "none",
                  C = 1,
                  gamma = 0.01,
                  coef0 = 0,
                  degree = 3,
                  shrinking = TRUE,
                  probability = FALSE,
                  cweights = NULL,
                  sweights = NULL,
                  cache_size = 100,
                  tol = 1e-3 ) {
    
    # check for errors
    
    if ( lib != "libsvm" ) { # || lib != "svmlight"
      stop("No such library. Avaiable are: libsvm")  
    }
    
    if ( kernel != "linear" && kernel != "poly" && kernel != "rbf" && kernel != "sigmoid" ) {
      stop("No such kernel type. Avaiable are: linear, poly, rbf, sigmoid")
    }
    
    if ( prep != "2e" && prep != "none" ) {
      stop("No such preprocess type, Avaiable are: 2e, none")
    }
    
    if ( mclass != "none" ) {
      stop("Sorry, multiclass is not yet supported")
    }
    
    if (C < 0 || gamma < 0 || degree < 1 ) {
      stop("Wrong parameters.")
    }
    
    config <- new(SVMConfiguration)
    config$x = x
    config$y = y
    
    config$setLibrary(lib)
    config$setKernel(kernel)
    config$setPreprocess(prep)
    
    config$C = C
    config$gamma = gamma
    config$coef0 = coef0
    config$degree = degree
    config$eps = tol
    config$cache_size = cache_size
    
    if ( !is.null(cweights) ) {
      config$setWeights(cweights)
    }
    
    if ( shrinking ) {
      config$shrinking = 1
    } else {
      config$shrinking = 0
    }
    
    if ( probability ) {
      config$probability = 1
    } else {
      config$probability = 0
    }
    
    client <- new(SVMClient, config)
    client 
  } 
    
  if ( !isGeneric("train") ) {
    setGeneric("train", function( object, ... ) standardGeneric("train") )
  }
  if ( !isGeneric("predict") ) {
    setGeneric("predict", function( object, x, ... ) standardGeneric("predict") )
  }
  if(!isGeneric("plot")){
    setGeneric("plot", function( object, dim1=1, dim2=2, ... ) standardGeneric("plot"))
  }
  if (!isGeneric("load_dataset")  ) {
    setGeneric( "load_dataset", function( object, x, ... ) standardGeneric("load_dataset") )
  }
  if (!isGeneric("dataset.X")  ) {
    setGeneric( "dataset.X", function( object, ... ) standardGeneric("dataset.X") )
  }
  if (!isGeneric("dataset.Y")  ) {
    setGeneric( "dataset.Y", function( object,  ... ) standardGeneric("dataset.Y") )
  }
  if (!isGeneric("params")) {
    setGeneric("params", function(object, 
                                  lib=FALSE,
                                  kernel=FALSE,
                                  preprocess=FALSE,
                                  C=FALSE,
                                  gamma=NULL,
                                  coef0=NULL,
                                  degree=FALSE,
                                  cache_size=FALSE,
                                  shrinking=NULL,
                                  probability=NULL,
                                  ...) standardGeneric("params"))
  }
  if (!isGeneric("params.lib")) {
    setGeneric("params.lib", function(object,x=FALSE,...) standardGeneric("params.lib"))
  }
  if (!isGeneric("params.kernel")) {
    setGeneric("params.kernel", function(object,x=FALSE,...) standardGeneric("params.kernel"))
  }
  if (!isGeneric("params.preprocess")) {
    setGeneric("params.preprocess", function(object,x=FALSE,...) standardGeneric("params.preprocess"))
  }
  if (!isGeneric("params.C")) {
    setGeneric("params.C", function(object,x=FALSE,...) standardGeneric("params.C"))
  }
  if (!isGeneric("params.gamma")) {
    setGeneric("params.gamma", function(object,x=NULL,...) standardGeneric("params.gamma"))
  }
  if (!isGeneric("params.degree")) {
    setGeneric("params.degree", function(object,x=FALSE,...) standardGeneric("params.degree"))
  }
  if (!isGeneric("params.coef0")) {
    setGeneric("params.coef0", function(object,x=NULL,...) standardGeneric("params.coef0"))
  }
  if (!isGeneric("params.cache_size")) {
    setGeneric("params.cache_size", function(object,x=FALSE,...) standardGeneric("params.cache_size"))
  }
  if (!isGeneric("params.shrinking")) {
    setGeneric("params.shrinking", function(object,x=NULL,...) standardGeneric("params.shrinking"))
  }
  if (!isGeneric("params.probability")) {
    setGeneric("params.probability", function(object,x=NULL,...) standardGeneric("params.probability"))
  }
  
  
  print.svm <- function(x) {
    print(sprintf("SVM object with: library: %s, kernel: %s, preprocess: %s, C: %.1f, gamma: %.3f, coef0: %.3f, degree: %d",
                  x$getLibrary(),
                  x$getKernel(),
                  x$getPreprocess(),
                  x$getC(),
                  x$getGamma(),
                  x$getCoef0(),
                  x$getDegree() ))
  }
  
  plot.svm <- function(object, dim1 = 1, dim2 = 2) {
    x = object$getX()
    if (dim1 > ncol(x) || dim2 > ncol(x)) {
      stop("Too large dimensions")
    }
    plot2d.svm(object,dim1,dim2)
  }
  
  train.svm <<- function(object) {
    object$train()
  }
  
  predict.svm <<- function(object, x) {
    object$predict(x)
    prediction = object$getPrediction()
    prediction
  }
  
  # dataset
  load.dataset <- function( object, x ) {
      object$setX( x[,c( seq(1, ncol(x) - 1,1))] )
      object$setY( x[,c(ncol(x))] )
  }
  
  dataset.X <- function(object) {
      object$getX()
  }
  
  dataset.Y <- function(object) {
      object$getY()
  }
  
  # Params
  params.svm <- function( object, 
                          lib=FALSE,
                          kernel=FALSE,
                          preprocess=FALSE,
                          C=FALSE,
                          gamma=NULL,
                          coef0=NULL,
                          degree=FALSE,
                          cache_size=FALSE,
                          shrinking=NULL,
                          probability=NULL
                          ) {
    check.trained(object)
    
    if (lib != FALSE) {
      if ( lib != "libsvm" ) { # || lib != "svmlight"
        stop("No such library. Avaiable are: libsvm") # 
      }
      object$setLibrary(lib)
    }
    if (kernel != FALSE) {
      if ( kernel != "linear" && kernel != "poly" && kernel != "rbf" && kernel != "sigmoid" ) {
        stop("No such kernel type. Avaiable are: linear, poly, rbf, sigmoid")
      }
      object$setKernel(kernel)
    }
    if (preprocess != FALSE) {
      if ( preprocess != "2e" && preprocess != "none" ) {
        stop("No such preprocess type, Avaiable are: 2e, none")
      }
      object$setPreprocess(preprocess)
    }
    if (C != FALSE && is.numeric(C)){
      object$setC(C)
    }
    if (!is.null(gamma) && is.numeric(gamma)) {
      object$setGamma(gamma)
    }
    if (!is.null(coef0) != FALSE && is.numeric(coef0)) {
      object$setCoef0(coef0)
    }
    if (degree != FALSE && is.numeric(degree)) {
      object$setDegree(degree)
    }
    if (cache_size != FALSE && is.numeric(cache_size)) {
      object$setCache(cache_size)
    }
    if (!is.null(shrinking) && is.logical(shrinking)) {
      if (shrinking) {
        object$setShrinking(1)
      }
      else {
        object$setShrinking(0)
      }
    }
    if (!is.null(probability) && is.logical(probability)) {
      if (probability) {
        object$setProbability(1)
      }
      else {
        object$setProbability(0)
      }
    }
  }
  
  params.svm.lib <- function(object, x=FALSE) {
    if (x == FALSE) {
      object$getLibrary()
    }
    else {
      if ( x != "libsvm" ) { # || x != "svmlight"
        stop("No such library. Avaiable are: libsvm")  
      }
      object$setLibrary(x)
    }
  }
  
  params.svm.kernel <- function(object, x=FALSE) {
    if (x == FALSE) {
      object$getKernel()
    }
    else {
      if ( x != "linear" && x != "poly" && x != "rbf" && x != "sigmoid" ) {
        stop("No such kernel type. Avaiable are: linear, poly, rbf, sigmoid")
      }
      object$setKernel(x)
    }
  }
  
  params.svm.prep <- function(object, x=FALSE) {
    if (x == FALSE) {
      object$getPreprocess()
    }
    else {
      if ( x != "2e" && x != "none" ) {
        stop("No such preprocess type, Avaiable are: 2e, none")
      }
      object$setPreprocess(x)
    }
  }
  
  params.svm.C <- function(object, x=FALSE) {
    if (x == FALSE) {
      object$getC()
    }
    else {
      check.trained(object)
      object$setC(x)
    }
  }
  
  params.svm.gamma <- function(object, x=NULL){
    if (is.null(x)) {
      object$getGamma()
    } 
    else {
      check.trained(object)
      object$setGamma(x)
    }
  }
  
  params.svm.degree <- function(object, x=FALSE){
    if (x == FALSE) {
      object$getDegree()
    }
    else {
      check.trained(object)
      object$setDegree(x)
    }
  }
  
  params.svm.coef0 <- function(object, x=NULL){
    if (is.null(x)) {
      object$getCoef0()
    }
    else {
      check.trained(object)
      object$setCoef0(x)
    }
  }
  
  params.svm.cache_size<- function(object, x=FALSE){
    if (x == FALSE) {
      object$getCache()
    }
    else {
      check.trained(object)
      object$setCache(x)
    }
  }
  
  params.svm.shrinking <- function(object, x=NULL){
    if (is.null(x)) {
      object$isShrinking()
    }
    else {
      check.trained(object)
      if (x==TRUE) {
        object$setShrinking(1)
      }
      else {
        object$setShrinking(0)
      }
    }
  }
  
  params.svm.probability <- function(object, x=NULL){
    if (is.null(x)) {
      object$isProbability()
    }
    else {
      check.trained(object)
      if (x) {
        object$setProbability(1)
      }
      else {
        object$setProbability(0)
      }
    }
  }
  
  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("train", "Rcpp_SVMClient", train.svm)
  setMethod("predict", signature("Rcpp_SVMClient", "matrix"), predict.svm )
  setMethod("plot", signature("Rcpp_SVMClient","numeric","numeric"), plot.svm)
  setMethod("plot", signature("Rcpp_SVMClient","missing","missing"), plot.svm)
  
  #dataset
  setMethod("load_dataset", signature("Rcpp_SVMClient", "matrix"), load.dataset)
  setMethod("dataset.X", signature("Rcpp_SVMClient"), dataset.X)
  setMethod("dataset.Y", signature("Rcpp_SVMClient"), dataset.Y)
  
  # params setters
  setMethod("params", signature("Rcpp_SVMClient", "ANY","ANY","ANY","ANY","ANY","ANY","ANY","ANY","ANY","ANY"), params.svm)
  setMethod("params.lib", signature("Rcpp_SVMClient", "character"), params.svm.lib)
  setMethod("params.kernel", signature("Rcpp_SVMClient", "character"), params.svm.kernel)
  setMethod("params.preprocess", signature("Rcpp_SVMClient", "character"), params.svm.prep)
  setMethod("params.C", signature("Rcpp_SVMClient", "numeric"), params.svm.C)
  setMethod("params.gamma", signature("Rcpp_SVMClient", "numeric"), params.svm.gamma)
  setMethod("params.degree", signature("Rcpp_SVMClient", "numeric"), params.svm.degree)
  setMethod("params.coef0", signature("Rcpp_SVMClient", "numeric"), params.svm.coef0)
  setMethod("params.cache_size", signature("Rcpp_SVMClient", "numeric"), params.svm.cache_size)
  setMethod("params.shrinking", signature("Rcpp_SVMClient", "logical"), params.svm.shrinking)
  setMethod("params.probability", signature("Rcpp_SVMClient", "logical"), params.svm.probability)
  
  #params getters
  setMethod("params.lib", signature("Rcpp_SVMClient", "missing"), params.svm.lib)
  setMethod("params.kernel", signature("Rcpp_SVMClient", "missing"), params.svm.kernel)
  setMethod("params.preprocess", signature("Rcpp_SVMClient", "missing"), params.svm.prep)
  setMethod("params.C", signature("Rcpp_SVMClient", "missing"), params.svm.C)
  setMethod("params.gamma", signature("Rcpp_SVMClient", "missing"), params.svm.gamma)
  setMethod("params.degree", signature("Rcpp_SVMClient", "missing"), params.svm.degree)
  setMethod("params.coef0", signature("Rcpp_SVMClient", "missing"), params.svm.coef0)
  setMethod("params.cache_size", signature("Rcpp_SVMClient", "missing"), params.svm.cache_size)
  setMethod("params.shrinking", signature("Rcpp_SVMClient", "missing"), params.svm.shrinking)
  setMethod("params.probability", signature("Rcpp_SVMClient", "missing"), params.svm.probability)

  
})


