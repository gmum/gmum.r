#' CEC
#' 
#' @title CEC()
#' 
#' @description Create CEC model object
#'
#' @export
#'
#' @rdname cec-methods
#'
#' @docType methods
#'
#' @param x numeric matrix of data
#' @param k initial number of clusters
#' @param method.type Gauss family
#' @param method.init method to initialize clusters
#' @param params.r radius for spherical family
#' @param params.cov covariance matrix for covariance family
#' @param params.centroids list of centroids
#' @param control.nstart how many times to perform algorithm
#' @param control.eps what change of value should terminate algorithm
#' @param control.itmax maximum number of iterations at each start
#' @param log.energy record collected energy of all clusters in each iteration
#' @param log.ncluster record number of clusters in each iteration
#' @param log.iters record number of iterations
CEC <- NULL

#' @title loop
#' 
#' @description Start loop of the algorithm where learning is done
#'
#' @docType methods
#'
#' @param c CEC model object
loop.cec <- NULL

#' @title entropy
#' 
#' @description Print result entropy of clustering found
#' 
#' @docType methods
#'
#' @param c CEC model object
entropy.cec <- NULL

#' @title y
#' @description Print labels assigned
#' 
#' @docType methods
#'
#' @param c CEC model object
y.cec <- NULL

#' @title centers
#' 
#' @description Print centers of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
centers.cec <- NULL

#' @title cov
#' 
#' @description Print covariances of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
cov.cec <- NULL

#' @title predictCluster
#' 
#' @description For given point predict cluster where it belongs
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictCluster.cec <- NULL

#' @title predictClusters
#' 
#' @description For given point print propabilities of belonging to each cluster
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictClusters.cec <- NULL

#' @title log.ncluster.cec
#' 
#' @description Print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object
log.ncluster.cec <- NULL

#' @title log.energy.cec
#' 
#' @description Print energy recorded that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object
log.energy.cec <- NULL

#' @title log.iters.cec
#' 
#' @description Print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object
log.iters.cec <- NULL

#' @title nstart
#' 
#' @description Print number of times CEC algorithm is to be performed. The best outcome
#' is chosen as result.
#'  
#' @docType methods
#'
#' @param c CEC model object
nstart.cec <- NULL

loadModule('cec', TRUE)

evalqOnLoad({

    CEC <<- function(x = NULL,
                     k = 0,
                     method.type = "",
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
      
      if (is.null(x))
        stop("Dataset is required!");
      
      if (k <= 0)
        stop("Number of clusters should be a positive integer!");
      
      if (control.nstart <= 0)
        stop("Number of starts should be a positive integer!");
      
      if (control.eps > 1.0 / k)
        stop("killThreshold " + contorl.eps + " is too high");  
      
      if (control.itmax <= 0)
        stop("Max number of iterations should be a positive integer!");
      
        config <- new(cecConfiguration)
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
      
        model <- new(cecModel, config)
        model
    }

    loop.cec <<- function(c) {
        c$loop()
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

    cov.cec <<- function(c) {
        c$cov()
    }

    predictCluster.cec <<- function(c, vec) {
        c$predict(vec)
    }

    predictClusters.cec <<- function(c, vec) {
        c$predict(vec,TRUE)
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
    
    setGeneric("loop", function(c) standardGeneric("loop"))
    setGeneric("entropy", function(c) standardGeneric("entropy"))
    setGeneric("energy", function(c) standardGeneric("energy"))
    setGeneric("y", function(c) standardGeneric("y"))
    setGeneric("centers", function(c) standardGeneric("centers"))
    setGeneric("cov", function(c) standardGeneric("cov"))
    setGeneric("predictCluster", function(c, ...) standardGeneric("predictCluster"))
    setGeneric("predictClusters", function(c, ...) standardGeneric("predictClusters"))
    setGeneric("log.ncluster", function(c) standardGeneric("log.ncluster"))
    setGeneric("log.energy", function(c) standardGeneric("log.energy"))
    setGeneric("log.iters", function(c) standardGeneric("log.iters"))
    setGeneric("nstart", function(c) standardGeneric("nstart"))
    
    setMethod("loop", "Rcpp_cecModel", loop.cec)
    setMethod("entropy", "Rcpp_cecModel", entropy.cec)
    setMethod("energy", "Rcpp_cecModel", energy.cec)
    setMethod("y", "Rcpp_cecModel", y.cec)
    setMethod("centers", "Rcpp_cecModel", centers.cec)
    setMethod("cov", "Rcpp_cecModel", cov.cec)
    setMethod("predictCluster", "Rcpp_cecModel", predictCluster.cec)
    setMethod("predictClusters", "Rcpp_cecModel", predictClusters.cec)
    setMethod("log.ncluster", "Rcpp_cecModel", log.ncluster.cec)
    setMethod("log.energy", "Rcpp_cecModel", log.energy.cec)
    setMethod("log.iters", "Rcpp_cecModel", log.iters.cec)
    setMethod("nstart", "Rcpp_cecModel", nstart.cec)
})
