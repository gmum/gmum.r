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
predict.svm.gmum <- NULL

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
#' @usage plot(svm, pca=TRUE, log="x")
#' @usage plot(svm, dim1=3, dim2=4)
#' 
#' @param pca Bool, use Principal Component Analysis for plot
#' @param dim1 (optional) Dimension of dataset to plot on x axis, by default 1
#' @param dim2 (optional) Dimension of dataset to plot on y axis, by default 2
#' @param log (optional) Use logarthic transformation for an axis, "x", "y", or "xy"
#' 
#' @rdname plot-dataset-methods
#' 
#' @docType plot
plot.svm <- NULL

#' @title summary
#' 
#' @description Prints short summary of a trained model.
#' 
#' @export
#' 
#' @usage summary(svm)
#' 
#' @param svm SVM object 
#' 
#' @rdname svm-summary-method
#' 
#' @docType plot
summary.svm <- NULL

loadModule('svm_wrapper', TRUE)

evalqOnLoad({

  SVM <<- function(formula     = NULL, 
                   x, 
                   y           = NULL,
                   lib         = "libsvm",             
                   kernel      = "linear",
                   prep        = "none",
                   mclass      = "none",
                   C           = 1,
                   gamma       = 0,
                   coef0       = 0,
                   degree      = 3,
                   shrinking   = TRUE,
                   probability = FALSE,
                   cweights    = NULL,
                   example_weights    = NULL,
                   cache_size  = 200,
                   tol         = 1e-3,
                   verbosity   = 4) {
    call <- match.call(expand.dots = TRUE)

    # Hacks to support caret
    if(!is.null(cweights) && cweights == FALSE){
      cweights = NULL
    }
    if(!is.null(example_weights) && example_weights == FALSE){
      example_weights = NULL
    }
    
    # check for errors
    if ( lib != "libsvm" && lib != "svmlight") { 
      stop(paste(GMUM_WRONG_LIBRARY, ": bad library, available are: libsvm, svmlight" )) 
      # log error No such library, available are: libsvm, svmlight
    }
    
    if (kernel != "linear" && kernel != "poly" && kernel != "rbf" && kernel != "sigmoid") {
      stop(paste(GMUM_WRONG_KERNEL, ": bad kernel" ))
      # log error: No such kernel type. available are: linear, poly, rbf, sigmoid
    }
    
    if (prep != "2e" && prep != "none") {
      stop(paste(GMUM_BAD_PREPROCESS, ": bad preprocess" ))
      # log erro No such preprocess type, available are: 2e, none
    }
    
    if (mclass != "none") {
      stop(paste(GMUM_NOT_SUPPORTED, ": multiclass" ))
      # log error: Sorry, multiclass is not yet supported
    }
    if (verbosity < 0 || verbosity > 6) {
      stop("Wrong verbosity level, should be from 0 to 6")
    }
    if (C < 0 || gamma < 0 || degree < 1) {
      stop(paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
      # log error: bad paramters
    }
    
    if (kernel=="linear" && degree != 1) {
      warning("Degree parameter is not used with linear kernel")
    }
    if (kernel=="linear" && gamma != 0.01) {
      warning("Gamma parameter is not used with linear kernel")
    }
    if (verbosity < 0 || verbosity > 6) {
      stop("Wrong verbosity level, should be from 0 to 6")
    }
    
    if (is.null(formula) && is.null(y)) {
      stop("Please provide either data and formula or data and lables")
    }
        
    if (is(x, "data.frame") && !is.null(formula)) {
      labels <- all.vars(update(formula, .~0))
      y <- data.matrix( x[, labels] )
      
      # I'm pretty sure this should bo done differently, and equally so I can't find how
      if (formula[3] == ".()") {
        x <- data.matrix( x[,names(x) != labels]  )
      }
      else {
        columns = all.vars(update(formula, 0~.))
        x <- data.matrix( x[, columns] )
      } 
    }
    else if (is(x, "matrix") && !is.null(formula)) {
      labels <- all.vars(update(formula, .~0))
      y <- x[, labels]
      
      # I'm pretty sure this should bo done differently, and equally so I can't find how
      if (formula[3] == ".()") {
        x <- x[, names(data) != labels]
      }
      else {
        columns = all.vars(update(formula, 0~.))
        x <- x[, columns]
      } 
    }
    else if(inherits(x, "Matrix")) {
      library("SparseM")
      library("Matrix")
      x <- as(x, "matrix.csr")
    }
    else if(inherits(x, "simple_triplet_matrix")) {
      library("SparseM")
      ind <- order(data$i, data$j)
      x <- new("matrix.csr",
               ra = x$v[ind],
               ja = x$j[ind],
               ia = as.integer(cumsum(c(1, tabulate(x$i[ind])))),
               dimension = c(x$nrow, data$ncol))
    }
    else if(inherits(x, "matrix.csr")) {
      library("SparseM")
    }
    else {
      stop("data is of a wrong class, please provide supported format: 
           matrix or data.frame for dense; 
           Matrix, simple_triplet_matrix or matrix.csr for sparse")
    }

    sparse <- inherits(x, "matrix.csr")
    
    if (sparse) {
      library("SparseM")
      if (is.null(y)) {
        stop("Please provide label vector y for sparse matrix classification")
      }
    }
    
    config <- new(SVMConfiguration)
    config$y <- data.matrix(y)
    
    # sparse 
    if (sparse) {
      config$sparse <- 1
      
      # OLD (for our conversion) TODO: Delete after rewrite in LibSVMRunner
      config$sp_x <- x@ra
      config$sp_row <- x@ia
      config$sp_col <- x@ja
      config$dim <- nrow(x)
      config$data_dim <- ncol(x)

      # NEW (for arma::sp_mat)  
      x <- as.matrix.csc(x)
      config$set_sparse_data(x@ja, x@ia, x@ra, nrow(x), ncol(x))
    }
    else {
      config$sparse <- 0
      config$x <- x
    }
    
    
    config$setLibrary(lib)
    config$setKernel(kernel)
    config$setPreprocess(prep)
    config$set_verbosity(verbosity)
    
    config$C <- C
    if (gamma == 0) {
      gamma <- 1/ncol(x) 
    }
    config$gamma <- gamma
    config$coef0 <- coef0
    config$degree <- degree
    config$eps <- tol
    config$cache_size <- cache_size
    
    if (!is.null(cweights)) {
      config$setWeights(cweights)
    }
    
    if (!is.null(example_weights)) {
      config$use_example_weights <- 1
      config$example_weights <- example_weights
    }
    
    if (shrinking) {
      config$shrinking <- 1
    } else {
      config$shrinking <- 0
    }
    
    if (probability) {
      config$probability <- 1
    } else {
      config$probability <- 0
    }
    
    client <- new(SVMClient, config)
    client$train()

    assign("call", call, client)
    client 
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
  
  summary.svm <<- function(object) {
    print(sprintf("Support Vector Machine, library: %s, kernel: %s, preprocess: %s",
                  object$getLibrary(), 
                  object$getKernel(), 
                  object$getPreprocess()))
    print(sprintf("%d classes with %d support vectors", 
                  object$get_number_class(), 
                  object$get_number_sv()))
  }
  
  plot.svm <<- function(x, mode="normal", dim1 = 1, dim2 = 2, log="") {
    if (x$isSparse()) {
      stop("Data is sparse")
    }
    if (mode != "pca" && mode != "normal" && mode != "contour" ) {
      stop("Wrong mode!") 
    }
    kernel <- x$getKernel()
    df <- data.frame( x$getX() )
    t <- x$getY()
    if (mode != "contour" && kernel == "linear") {
      w <- c(x$getW())
    }
    if (mode == "pca" && kernel == "linear") {
      pca_data = prcomp(df, scale=TRUE)
      scores = data.frame(df, pca_data$x[,1:2])
      w <- w %*% pca_data$rotation
      A <- w[1]
      B <- w[2]
      C <- x$getBias()
      
      s <- -A/B
      int <- -C/B
      
      pl <- ggplot() +
        geom_point(data=scores, aes(PC1, PC2), colour=factor(t+2)) + geom_abline(slope=s, intercept=int)
      plot(pl)
    }
    else if (mode == "normal" && kernel == "linear") { 
      if (dim1 > ncol(df) || dim2 > ncol(df)) {
        stop("Too large dimensions")
      }
      A <- w[1]
      B <- w[2]
      C <- x$getBias()      
      s <- -A/B
      int <- -C/B
    
      t <- replace(t, t==1, 'blue')
      t <- replace(t, t==-1, 'red')
      
      pl <- ggplot() + geom_point(data=df, aes(X1, X2), colour=t)  +
        geom_abline(slope=s, intercept=int)
      plot(pl)
    }
    else if (mode == "contour" || kernel != "linear") {    # test mode
      warning("This is experimental mode, it will change your SVM's data!")
      temp_target <- x$getY()
      x_col <- df[colnames(df)[1]]
      y_col <- df[colnames(df)[2]]
      
      x_max <- max(x_col)
      x_min <- min(x_col) 
      y_max <- max(y_col)
      y_min <- min(y_col)
      
      x_axis <- seq(from=x_min, to=x_max, length.out=300)
      y_axis <- seq(from=y_min, to=y_max, length.out=300)
      grid <- data.frame(x_axis,y_axis)
      grid <- expand.grid(x=x_axis,y=y_axis)
      target <- predict(x, grid)
      
      if (x$areExamplesWeighted()) {
        df['sizes'] <- x$getExampleWeights()
      }
      else {
        df['sizes'] <- 0.1
      }
      
      t <- replace(t, t==1, 'blue')
      t <- replace(t, t==-1, 'red')
      grid["target"] <- target
      
      pl <- ggplot()+ 
        geom_tile(data=grid, aes(x=x,y=y, fill=factor(target))) + theme(legend.position="none") +
        geom_point(data=df, aes(X1, X2, size=sizes), colour=t) + scale_size_continuous(range = c(3, 6))
      plot(pl)
    }
  }
  
  predict.svm.gmum <<- function(object, x) {
    if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric") && !is(x,"matrix.csr") ) {
      stop("Wrong target class, please provide data.frame, matrix or numeric vector")
    }
    if (!object$isSparse()) {
      if (!is(x, "matrix") && !is(x, "data.frame")) {
        stop("Please provide matrix or data.frame")
      }
      if (!is(x, "matrix")) {
        x <- data.matrix(x)
      }
      object$predict(x)
    }
    else {
      if (!is(x, "matrix.csr")) {
        stop("Please provide sparse matrix")
      }
      x <- as.matrix.csc(x)
      object$sparse_predict(x@ja, x@ia, x@ra, nrow(x), ncol(x))
    }
    prediction <- object$getPrediction()
    prediction
  }

  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("predict", "Rcpp_SVMClient", predict.svm.gmum)
  setMethod("plot", "Rcpp_SVMClient",  plot.svm)
  setMethod("summary", "Rcpp_SVMClient", summary.svm)
  setMethod("show", "Rcpp_SVMClient", summary.svm)

})


