library(ggplot2)

#' @title SVM
#' 
#' @export
#' 
#' @description Create and train SVM model object. If any parameter will be omitted a default value will be used
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

#' @title Predict
#' 
#' @description Returns predicted classes for provided test examples.
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
#' @description Draws choosen dimenstions from a dataset on 2D plot coloring by class. NOTE: This function will be change to a package default function.
#' 
#' @export
#' 
#' @usage plot(svm, dim1, dim2)
#' 
#' @param dim1 (optional) Dimension of x to plot on x axis, by default 1
#' 
#' @param dim2 (optional) Dimension of x to plot on y axis, by default 2
#' 
#' @rdname plot-dataset-methods
#' 
#' @docType plot
plot.svm <- NULL

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

loadModule('svm_wrapper', TRUE)

evalqOnLoad({

  SVM <<- function( formula, 
                    data, 
                    lib = "libsvm",             
                    kernel = "linear",
                    prep = "none",
                    mclass = "none",
                    C = 1,
                    gamma = 0.01,
                    coef0 = 0,
                    degree = 1,
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
      stop("Wrong parameters")
    }
    
    if (kernel=="linear" && degree != 1) {
      warning("Degree parameter is not used with linear kernel")
    }
    if (kernel=="linear" && gamma != 0.01) {
      warning("Gamma parameter is not used with linear kernel")
    }
    
    labels = all.vars(update(formula,.~0))
    x <- NULL
    y <- NULL
    
    if (is(data, "data.frame")) {
      y = data.matrix( data[,labels] )
      
      # I'm pretty sure this should bo done differently, and equally so I can't find how
      if (formula[3] == ".()"  ) {
        x = data.matrix( data[,names(data) != labels]  )
      }
      else {
        columns = all.vars(update(formula,0~.))
        x = data.matrix( data[,columns] )
      } 
    }
    else if (is(data, "matrix")) {
      y = data[,labels]
      
      # I'm pretty sure this should bo done differently, and equally so I can't find how
      if (formula[3] == ".()"  ) {
        x = data[,names(data) != labels]
      }
      else {
        columns = all.vars(update(formula,0~.))
        x = data[,columns]
      } 
    }
    else {
      stop("data is of a wrong class, please provide data.frame or matrix")
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
    client$train()

    client 
  } 
    
  if ( !isGeneric("predict") ) {
    setGeneric("predict", function( object, x, ... ) standardGeneric("predict") )
  }

  if (!isGeneric("dataset.X")  ) {
    setGeneric( "dataset.X", function( object, ... ) standardGeneric("dataset.X") )
  }
  if (!isGeneric("dataset.Y")  ) {
    setGeneric( "dataset.Y", function( object,  ... ) standardGeneric("dataset.Y") )
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
  
  plot.svm <<- function(x, dim1 = 1, dim2 = 2) {
    df =  data.frame( x$getX() ) 
    if (dim1 > ncol(df) || dim2 > ncol(df)) {
      stop("Too large dimensions")
    }
    t = x$getY()
    x = df[,dim1]
    y = df[,dim2]
    qplot(data=df, x=x, y=y, color=t) # + scale_colour_gradientn(colours=rainbow(2),breaks = c(2,4))
  }
  
  predict.svm <<- function(object, x) {
    if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
      stop("Wrong target class, please provide data.frame, matrix or numeric vector")
    }
    
    if (!is(x, "matrix")) {
      x = data.matrix(x)
    }
    object$predict(x)
    prediction = object$getPrediction()
    prediction
  }
  
  dataset.X <- function(object) {
      object$getX()
  }
  
  dataset.Y <- function(object) {
      object$getY()
  }
  
  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("predict", signature("Rcpp_SVMClient", "ANY"), predict.svm )
  setMethod("plot", "Rcpp_SVMClient",  plot.svm)

  #dataset
  setMethod("dataset.X", signature("Rcpp_SVMClient"), dataset.X)
  setMethod("dataset.Y", signature("Rcpp_SVMClient"), dataset.Y)
    
})


