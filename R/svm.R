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
summary.svm <- NULL

# Support for caret
caret.gmumSvmRadial <- NULL
caret.gmumSvmLinear <- NULL
caret.gmumSvmPoly <- NULL


SVM.formula <- NULL
SVM.default <- NULL

loadModule('svm_wrapper', TRUE)

evalqOnLoad({

  SVM <<- function(x, ...)
    UseMethod("SVM")
  
  SVM.formula <<- function(formula, data, ...) {
    
    call <- match.call(expand.dots = TRUE)
    
    if (!inherits(formula, "formula")) stop("Please provide valid formula for this method.")
    if(inherits(data, "Matrix") || inherits(x, "simple_triplet_matrix") || inherits(x, "matrix.csr")) 
      stop("Please provide dense data for this method")
  
    labels <- all.vars(update(formula, .~0))
    y <- data[, labels]
    
    # better way?
    if (formula[3] == ".()") {
      x <- data[, colnames(data) != labels]
    }
    else {
      columns <- all.vars(update(formula, 0~.))
      x <- data[, columns]
    } 
    
    if (is.data.frame(x)) x <- data.matrix(x)

    ret <- SVM.default(x, y, ...)
    call[[1]] <- as.name("SVM")
    assign("call", call, ret)
    
    return(ret)
  }
  
  SVM.default <<- 
  function(x,
           y,
           lib         = "libsvm",             
           kernel      = "linear",
           prep        = "none",
           mclass      = "none",
           transductive.learning = FALSE,
           transductive.posratio = -1.,
           C           = 1,
           gamma       = if (is.vector(x)) 1 else 1 / ncol(x),
           coef0       = 0,
           degree      = 3,
           shrinking   = TRUE,
           probability = FALSE,
           class.weights    = NULL,
           example.weights    = NULL,
           cache_size  = 200,
           tol         = 1e-3,
           verbosity   = 4,
           seed = NULL) {
    
    call <- match.call(expand.dots = TRUE)
    call[[1]] <- as.name("SVM")

    # check for errors
    if ( lib != "libsvm" && lib != "svmlight") stop(paste(GMUM_WRONG_LIBRARY, ": bad library, available are: libsvm, svmlight" ))  
    if ( lib != "svmlight" && transductive.learning) stop(paste(GMUM_WRONG_LIBRARY, ": bad library, transductive learning is supported only by svmlight" ))  
    if (kernel != "linear" && kernel != "poly" && kernel != "rbf" && kernel != "sigmoid") stop(paste(GMUM_WRONG_KERNEL, ": bad kernel" ))
    if (prep != "2e" && prep != "none") stop(paste(GMUM_BAD_PREPROCESS, ": bad preprocess" ))
    if (mclass != "none") stop(paste(GMUM_NOT_SUPPORTED, ": multiclass" ))
    if (verbosity < 0 || verbosity > 6) stop("Wrong verbosity level, should be from 0 to 6")
    if (C < 0 || gamma < 0 || degree < 1) stop(paste(GMUM_WRONG_PARAMS, ": bad SVM parameters" ))
    if (verbosity < 0 || verbosity > 6) stop("Wrong verbosity level, should be from 0 to 6")
      
    # check data
    if(nrow(x) != length(y)) stop("x and y have different lengths")
    if(inherits(x, "Matrix")) {
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
    else if(is.data.frame(x)) {
      x <- data.matrix(x)
    }
    else if (!is.matrix(x)){
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
    
    if (!is.vector(y) && !is.factor(y)) stop("y is of a wrong class, please provide vector or factor")
    
    levels <- NULL
    if (is.factor(y)){
      levels <- levels(y)
    }else{
      # Standarizing, easier for library
      y <- as.factor(y)
      levels <- levels(y)
      warning("It is recommended to pass y as factor")
    }
    
    # Binary classification or 2 classes + unlabeled (for transductive learning)
    if( (length(levels) != 2 && !transductive.learning) || 
         (length(levels) != 3 && transductive.learning)){
      stop("Please pass correct (binary) number of classes or 3 for transductive")
    }

    # Decide what label is used for unlabeled examples
    unlabeled.level = "TR"
    unlabeled.level = "TR"
    if(transductive.learning){
      if(! ("TR" %in% levels || "0" %in% levels ) ){
        stop("Please include TR or 0 factor in levels for transductive learning")
      }
      if("TR" %in% levels && "0" %in% levels ){
        stop("Couldn't deduce which label to use for transductive learning")
      }
      
      if("TR" %in% levels){
        unlabeled.level <- "TR"
      }else{
        unlabeled.level <- "0"
      }
    }
    # This ugly block of code ensures -1, 1 and 0 classes.
    # Contribution to simplifying this are welcome :)
    if(transductive.learning){
      # Erasing TR from levels. We will never return it
      levels = levels[levels != unlabeled.level] 
      indexes.unlabeled <- y == unlabeled.level
      
      y <- as.integer(y)
      
      z <- y[!indexes.unlabeled]
      z <- as.integer(factor(z, levels=levels))
      z[z==1] = -1
      z[z==2] = 1
      y[indexes.unlabeled] <- 0
      y[!indexes.unlabeled] <- z
    }else{
      y <- as.integer(y) # Standarization, omits 0!
      y[y==1] = -1 # Standarize it further!
      y[y==2] = 1
    }
    
    
    config <- new(SVMConfiguration)
    config$y <- data.matrix(y)
    
    config$use_transductive_learning = transductive.learning
    config$transductive_posratio = transductive.posratio
    
    # sparse 
    if (sparse) {
      config$sparse <- 1
      
      # NEW (for arma::sp_mat)  
      x <- as.matrix.csc(x)
      config$set_sparse_data(x@ja, x@ia, x@ra, nrow(x), ncol(x), TRUE)
    }
    else {
      config$sparse <- 0
      config$x <- x
    }
    
    if(!is.null(seed)){
      config$setSeed(seed)
    }
    config$setLibrary(lib)
    config$setKernel(kernel)
    config$setPreprocess(prep)
    config$set_verbosity(verbosity)
    
    config$C <- C
    config$gamma <- gamma
    config$coef0 <- coef0
    config$degree <- degree
    config$eps <- tol
    config$cache_size <- cache_size
    
    if (!is.null(class.weights) && !is.logical(class.weights)) {
      
      if(is.null(names(class.weights)) && class.weights != 'auto'){
          stop("Please provide class.weights as named (by classes) list or vector or 'auto'")
      }
      
      if (is.character(class.weights) && class.weights == "auto") {
        # sklearns heuristic automatic class weighting
        counts <- hist(y, breaks=2, plot=FALSE)$counts
        inv_freq <- 1 / counts
        weights <- inv_freq / mean(inv_freq)
        config$setClassWeights(weights)
      }
      else {
        # Maps name -> index that is feed into SVM
        # Note: factor is transformed such that class -> index in levels of factor
        class.labels.indexes <- sapply(names(class.weights), function(cls){ which(levels== cls)[1] })
        # Standarize for all libraries (so if passed list("2"=1, "1"=3) it is reversed)
        class.weights <- class.weights[order(class.labels.indexes)] 
        # We always pass numeric, so it will work if it is the case
        if(!is.numeric(y)){
          stop("[DEV] breaking change, please fix")
        }
        config$setClassWeights(as.numeric(class.weights))
      }
    }
    
    if (!is.null(example.weights) && !is.logical(example.weights)) {
      config$use_example_weights <- 1
      config$example_weights <- example.weights
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
    assign("levels", levels, client)
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
    
    if(any(prediction == 0) || length(unique(prediction)) > length(object$levels)){
       stop("Failed prediction, returned too many unique labels from library.")
    }
    
    
    if(!is.null(object$levels)){
      # This line works because we do as.numeric() which transforms into 1 and 2
      # And we expect SVM to return same labels as passed
      if(length(object$levels) == 2){
         # Binary case
         prediction <- factor(object$levels[(prediction+1)/2 + 1], levels = object$levels)
      }else{
         prediction <- factor(object$levels[prediction], levels = object$levels)
      }
     
    }
    
    prediction
  }

  setMethod("print", "Rcpp_SVMClient", print.svm)
  setMethod("predict", "Rcpp_SVMClient", predict.svm.gmum)
  setMethod("plot", "Rcpp_SVMClient",  plot.svm)
  setMethod("summary", "Rcpp_SVMClient", summary.svm)
  setMethod("show", "Rcpp_SVMClient", summary.svm)

  # Add (very basic) support for caret
  
  copy <- function(x) x
  
  gmum.r.svm.radial.params=c("C", "gamma")
  gmum.r.svm.radial.params.classes=c("double", "double")
  
  gmum.r.svm.linear.params=c("C")
  gmum.r.svm.linear.params.classes=c("double")
  
  gmum.r.svm.poly.params=c("C", "gamma", "degree", "coef0")
  gmum.r.svm.poly.params.classes=c("double", "double", "double", "double")
  
  caret.gmumSvmRadial <<- list(label = "gmum.r.svmRadial",
                               library = c("gmum.r"),
                               type = "Classification",
                               parameters = data.frame(parameter = gmum.r.svm.radial.params,
                                                       class = gmum.r.svm.radial.params.classes,
                                                       label = gmum.r.svm.radial.params),
                               grid = function(x, y, len = NULL) {
                                 # We pass tuning grid manually. 
                                 expand.grid(C=10^(-7:11),
                                             gamma=10^(-10:10))
                               },
                               fit = function(x, y, wts, param, lev, last, classProbs, ...) { 
                                 ## First fti the pls model, generate the training set scores,
                                 ## then attach what is needed to the random forest object to 
                                 ## be used late
                                 x.df = as.data.frame(x)
                                 x.df$y = as.numeric(y)
                                 param$kernel = 'linear'
                                 
                                 if(is.null(param$gamma)){
                                   param$gamma = 1
                                 }else{
                                   param$kernel = 'rbf'
                                 }
                                 if(is.null(param$degree)){
                                   param$degree = 3
                                 }else{
                                   param$kernel = 'poly'
                                 }
                                 if(is.null(param$coef0)){
                                   param$coef0 = 0
                                 }
                                 
                                 return(gmum.r::SVM(
                                   x,
                                   y,
                                   C = param$C,
                                   gamma = param$gamma,
                                   degree = param$degree,
                                   coef0 = param$coef0,
                                   probability = classProbs,
                                   kernel = param$kernel,
                                   ...
                                 ))
                               },
                               predict = function(modelFit, newdata, submodels = NULL) {  
                                 as.factor(predict(modelFit, newdata))
                               },
                               prob = function(modelFit, newdata, submodels = NULL) {  
                                 predict(modelFit, newdata)
                               },
                               varImp = NULL,
                               levels = function(x) {levels(x$getY())},
                               sort = function(x) x[order(x[,1]),]
  )
  
  caret.gmumSvmLinear.loc <- copy(caret.gmumSvmRadial)
  caret.gmumSvmPoly.loc <- copy(caret.gmumSvmRadial)
  
  
  caret.gmumSvmLinear.loc$parameters <- data.frame(parameter = gmum.r.svm.linear.params,
                                                class = gmum.r.svm.linear.params.classes,
                                                label = gmum.r.svm.linear.params)
  
  caret.gmumSvmLinear.loc$grid <- function(x, y, len = NULL) {
    expand.grid(C=10^(-7:11))
  }
  
  caret.gmumSvmPoly.loc$grid <- function(x, y, len = NULL) {
    expand.grid(C=10^(-7:11), gamma= 10^(-10:10), coef0=c(0,1,10), degree=c(2,3,4) )
  }
  
  
  caret.gmumSvmPoly.loc$parameters <- data.frame(parameter = gmum.r.svm.poly.params,
                                             class = gmum.r.svm.poly.params.classes,
                                             label = gmum.r.svm.poly.params)
  
  caret.gmumSvmPoly <<- caret.gmumSvmPoly.loc
  caret.gmumSvmLinear <<- caret.gmumSvmLinear.loc
  
})


