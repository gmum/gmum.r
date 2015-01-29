#' CEC
#' 
#' @title CEC
#' 
#' @description Create CEC model object
#'
#' @export
#'
#' @rdname cec-methods
#'
#' @docType methods
#'
#' @param x Numeric matrix of data.
#' @param k Initial number of clusters.
#' @param method.type Type of clustering (Gauss family).
#' @param method.init Method to initialize clusters.
#' @param params.r Radius for spherical family.
#' @param params.cov Covariance matrix for covariance family.
#' @param params.centroids List of centroids.
#' @param control.nstart How many times to perform algorithm.
#' @param control.eps What change of value should terminate algorithm.
#' @param control.itmax Maximum number of iterations at each start.
#' @param log.energy Records collected energy of all clusters in each iteration.
#' @param log.ncluster Records number of clusters in each iteration.
#' @param log.iters Records number of iterations.
#' 
#' @usage CEC(k=3, x=dataset)
#' @usage CEC(k=3, x=dataset, control.nstart=10, method.type='sphere')
#' 
CEC <- NULL

#' @title run
#' 
#' @description Start loop of the algorithm where learning is done
#'
#' @docType methods
#'
#' @param c CEC model object.
#' 
run.cec <- NULL

#' @title entropy
#' 
#' @description Print result entropy of clustering found
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
entropy.cec <- NULL

#' @title y
#' 
#' @description Print labels assigned
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
y.cec <- NULL

#' @title centers
#' 
#' @description Print centers of clusters
#'
#' @docType methods
#' 
#' @param c CEC model object.
#' 
centers.cec <- NULL

#' @title covMatrix
#' 
#' @description Print covariances of clusters
#'
#' @docType methods
#'
#' @param c CEC model object.
#' 
covMatrix.cec <- NULL

#' @title predictCluster
#' 
#' @description Returns cluster where belong given point.
#' 
#' @rdname cec-predict-methods
#' 
#' @export
#' 
#' @docType methods
#' 
#' @param c Trained CEC model object.
#' @param vec Given point.
#' 
predictCluster.cec <- NULL

#' @title predictClusters
#' 
#' @description Returns propabilities of belonging to each cluster for given point.
#' 
#' @rdname cec-predict-methods
#' 
#' @export
#' 
#' @docType methods
#' 
#' @param c Trained CEC model object.
#' @param vec Given point.
#' 
predictClusters.cec <- NULL

#' @title log.ncluster.cec
#' 
#' @description Print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.ncluster.cec <- NULL

#' @title log.energy.cec
#' 
#' @description Print energy that has been recorded at each stage of learning.
#' Data is recorded only if you have chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.energy.cec <- NULL

#' @title log.iters.cec
#' 
#' @description Print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
log.iters.cec <- NULL

#' @title nstart
#' 
#' @description Print number of times CEC algorithm is to be performed. The best outcome
#' is chosen as result.
#'  
#' @docType methods
#'
#' @param c CEC model object.
#' 
nstart.cec <- NULL

loadModule('cec', TRUE)

evalqOnLoad({
  CEC <<- function(x = NULL,
                   k = 0,
                   method.type = "standard",
                   method.init = "kmeans++",
                   params.r = 0,
                   params.cov = matrix(0),
                   params.centroids = NULL,
                   params.mix = NULL,
                   params.function = "",
                   control.nstart = 1,
                   control.eps = 1e-4,
                   control.itmax = 1,
                   log.energy = FALSE,
                   log.ncluster= FALSE,
                   log.iters = FALSE){
    
    # check for errors
    call <- match.call(expand.dots = TRUE)
    
    if (is.null(x))
      stop("Dataset is required!");
    
    if(is(x, "data.frame")){
      x = data.matrix(x);
    }
    
    if (k <= 0)
      stop("Number of clusters should be a positive integer!");
    
    if (control.nstart <= 0)
      stop("Number of starts should be a positive integer!");
    
    if (control.eps > 1.0 / k)
      stop("killThreshold " + contorl.eps + " is too high!");  
    
    if (control.itmax <= 0)
      stop("Maximum number of iterations should be a positive integer!");
    
    if(is(params.cov, "data.frame")){
     params.cov = data.matrix(params.cov);
    }
    
    config <- new(CecConfiguration)
    config$setDataSet(x)
    config$setEps(control.eps)      
    config$setMix(params.mix) 
    config$setNrOfClusters(k)
    config$setLogEnergy(log.energy)
    config$setLogCluster(log.ncluster)      
    config$setNstart(control.nstart)
    config$setCentroids(params.centroids)
    config$setMethodInit(method.init)              
    config$setMethodType(method.type)
    config$setCov(params.cov)
    config$setR(params.r)
    config$setFunction(params.function)
    config$setItmax(control.itmax)
    config$setIters(log.iters)
    
    model <- new(CecModel, config)

    assign("call", call, model)
    model
  }
  
  run.cec <<- function(c) {
    c$run()
  }
  
  entropy.cec <<- function(c) {
    c$entropy()
  }
  
  energy.cec <<- function(c) {
    c$energy()
  }
  
  y.cec <<- function(c) {
    c$y()
  }
  
  centers.cec <<- function(c) {
    c$centers()
  }
  
  covMatrix.cec <<- function(c) {
    c$covMatrix()
  }
  
  predictCluster.cec <<- function(c, x) {
    if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
      stop("Wrong target class, please provide data.frame, matrix or numeric vector")
    }
    
    if (!is(x, "matrix")) {
      x = data.matrix(x)
    }
    c$predict(x)
  }
  
  predictClusters.cec <<- function(c, x) {
    if ( !is(x, "data.frame") && !is(x, "matrix") && !is(x,"numeric")  ) {
      stop("Wrong target class, please provide data.frame, matrix or numeric vector")
    }
    
    if (!is(x, "matrix")) {
      x = data.matrix(x)
    }
    c$predict(x, TRUE)
  }
    
    log.ncluster.cec <<- function(c) {
      c$log.ncluster()
    }
    
    log.energy.cec <<- function(c) {
      c$log.energy()
    }
    
    log.iters.cec <<- function(c) {
      c$log.iters()
    }
    
    nstart.cec <<- function(c) {
      c$nstart()
    }
    
    setGeneric("run", function(c) standardGeneric("run"))
    setGeneric("entropy", function(c) standardGeneric("entropy"))
    setGeneric("energy", function(c) standardGeneric("energy"))
    setGeneric("y", function(c) standardGeneric("y"))
    setGeneric("centers", function(c) standardGeneric("centers"))
    setGeneric("covMatrix", function(c) standardGeneric("covMatrix"))
    setGeneric("predictCluster", function(c, ...) standardGeneric("predictCluster"))
    setGeneric("predictClusters", function(c, ...) standardGeneric("predictClusters"))
    setGeneric("log.ncluster", function(c) standardGeneric("log.ncluster"))
    setGeneric("log.energy", function(c) standardGeneric("log.energy"))
    setGeneric("log.iters", function(c) standardGeneric("log.iters"))
    setGeneric("nstart", function(c) standardGeneric("nstart"))
    
    setMethod("run", "Rcpp_CecModel", run.cec)
    setMethod("entropy", "Rcpp_CecModel", entropy.cec)
    setMethod("energy", "Rcpp_CecModel", energy.cec)
    setMethod("y", "Rcpp_CecModel", y.cec)
    setMethod("centers", "Rcpp_CecModel", centers.cec)
    setMethod("covMatrix", "Rcpp_CecModel", covMatrix.cec)
    setMethod("predictCluster", "Rcpp_CecModel", predictCluster.cec)
    setMethod("predictClusters", "Rcpp_CecModel", predictClusters.cec)
    setMethod("log.ncluster", "Rcpp_CecModel", log.ncluster.cec)
    setMethod("log.energy", "Rcpp_CecModel", log.energy.cec)
    setMethod("log.iters", "Rcpp_CecModel", log.iters.cec)
    setMethod("nstart", "Rcpp_CecModel", nstart.cec)
    
    setMethod("predict", "Rcpp_CecModel", function(object, vec) {
      object$predict(vec)
    })
})
