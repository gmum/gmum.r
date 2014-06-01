# mod <- Module("svm_wrapper")
loadModule('svm_wrapper', TRUE)

breast_cancer.path = "breast_cancer.data"

#print.svm <- NULL

SVM <- function(x = svm.breast_cancer.x(), # default data set here
                y = svm.breast_cancer.y(), # default labels here
                lib = "libsvm",
                kernel = "rbf",
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

evalqOnLoad({
    
  if ( !isGeneric("train") ) {
    setGeneric("train", function( object, ... ) standardGeneric("train") )
  }
  
  if ( !isGeneric("predict") ) {
    setGeneric("predict", function( object, x, ... ) standardGeneric("predict") )
  }
  if (!isGeneric("load_dataset")  ) {
    setGeneric( "load_dataset", function( object, x, ... ) standardGeneric("load_dataset") )
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
  
  train.svm <- function(object) {
    object$run()
  }
  
  predict.svm <- function(object, x) {
    object$predict(x)
    prediction = object$getPrediction()
    prediction
  }
  
  load.dataset <- function( object, x ) {
    object$setX( x[,c( seq(1, ncol(x) - 1,1))] )
    object$setY( x[,c(ncol(x))] )
  }
  
  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("train", "Rcpp_SVMClient", train.svm)
  setMethod("predict", signature("Rcpp_SVMClient", "numeric"), predict.svm )
  setMethod("load_dataset", signature("Rcpp_SVMClient", "matrix"), load.dataset)
  
  
  
})


test_norm <- function(x){
  
  con = mod$SVMConfiguration
  cli = mod$SVMClient
  config = new(con)
  config$data = x
  config$createParams( "poly", "libsvm", "norm", 2, 0.1, 0.0 )
  
  client = new(cli, config)
  client$run()
  result = client$getResult()
  return(result)
}