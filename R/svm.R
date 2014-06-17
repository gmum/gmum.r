
SVM <- NULL

train.svm <- NULL

predict.svm <- NULL

loadModule('svm_wrapper', TRUE)
breast_cancer.path = "./data/svm/breast_cancer.data"

evalqOnLoad({

  SVM <<- function(x = svm.breast_cancer.x(), 
                  y = svm.breast_cancer.y(),
                  lib = "libsvm",
                  kernel = "linear",
                  prep = "none",
                  mclass = "none",
                  C = 1,
                  gamma = 0,
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
      stop("No such library. Avaiable are: libsvm") # 
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
    setGeneric("plot", function(object, dim1=1, dim2=2, ...) standardGeneric("plot"))
  }
  if (!isGeneric("load_dataset")  ) {
    setGeneric( "load_dataset", function( object, x, ... ) standardGeneric("load_dataset") )
  }
  if (!isGeneric("params.C")) {
    setGeneric("params.C", function(object,x,...) standardGeneric("params.C"))
  }
  if (!isGeneric("params.gamma")) {
    setGeneric("params.gamma", function(object,x,...) standardGeneric("params.gamma"))
  }
  if (!isGeneric("params.degree")) {
    setGeneric("params.degree", function(object,x,...) standardGeneric("params.degree"))
  }
  if (!isGeneric("params.coef0")) {
    setGeneric("params.coef0", function(object,x,...) standardGeneric("params.coef0"))
  }
  if (!isGeneric("params.cache_size")) {
    setGeneric("params.cache_size", function(object,x,...) standardGeneric("params.cache_size"))
  }
  if (!isGeneric("params.shrinking")) {
    setGeneric("params.shrinking", function(object,x,...) standardGeneric("params.shrinking"))
  }
  if (!isGeneric("params.probability")) {
    setGeneric("params.probability", function(object,x,...) standardGeneric("params.probability"))
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
  
  load.dataset <- function( object, x ) {
    object$setX( x[,c( seq(1, ncol(x) - 1,1))] )
    object$setY( x[,c(ncol(x))] )
  }
  
  # Params
  params.svm.C <- function(object, x) {
    check.trained(object)
    object$setC(x)
  }
  
  params.svm.gamma <- function(object, x){
    check.trained(object)
    object$setGamma(x)
  }
  
  params.svm.degree <- function(object, x){
    check.trained(object)
    object$setDegree(x)
  }
  
  params.svm.coef0 <- function(object, x){
    check.trained(object)
    object$setCoef0(x)
  }
  
  params.svm.cache_size<- function(object, x){
    check.trained(object)
    object$setCache(x)
  }
  
  params.svm.shrinking <- function(object, x){
    check.trained(object)
    if (x) {
      object$setShrinking(1)
    }
    else {
      object$setShriking(0)
    }
  }
  
  params.svm.probability <- function(object, x){
    check.trained(object)
    if (x) {
      object$setProbability(1)
    }
    else {
      object$setProbability(0)
    }
  }
  
  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("train", "Rcpp_SVMClient", train.svm)
  setMethod("predict", signature("Rcpp_SVMClient", "matrix"), predict.svm )
  setMethod("plot", signature("Rcpp_SVMClient","numeric","numeric"), plot.svm)
  setMethod("plot", signature("Rcpp_SVMClient","missing"), plot.svm)
  setMethod("load_dataset", signature("Rcpp_SVMClient", "matrix"), load.dataset)
  
  setMethod("params.C", signature("Rcpp_SVMClient", "numeric"), params.svm.C)
  setMethod("params.gamma", signature("Rcpp_SVMClient", "numeric"), params.svm.gamma)
  setMethod("params.degree", signature("Rcpp_SVMClient", "numeric"), params.svm.degree)
  setMethod("params.coef0", signature("Rcpp_SVMClient", "numeric"), params.svm.coef0)
  setMethod("params.cache_size", signature("Rcpp_SVMClient", "numeric"), params.svm.cache_size)
  setMethod("params.shrinking", signature("Rcpp_SVMClient", "logical"), params.svm.shrinking)
  setMethod("params.probability", signature("Rcpp_SVMClient", "logical"), params.svm.probability)
  
  
  
})


