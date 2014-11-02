#' CEC()
#' 
#' @title CEC()
#' Create CEC model object
#'
#' @export
#'
#' @rdname cec-methods
#'
#' @docType methods
#'
#' @param x dataset
#' @param k initial number of clusters
#' @param method.type Gauss family
#' @param method.init method to initialize clusters
#' @param params.r radius for spherical family
#' @param params.cov covariance matrix for covariance family
#' @param params.centroids list of centroids
#' @param control.nstart how many times to perform algorithm
#' @param control.eps what change of value should terminate algorithm
#' @param control.itmax maximum number of iterations
#' @param log.energy record collected energy of all clusters in each iteration
#' @param log.ncluster record number of clusters in each iteration
#' @param log.iters record number of iterations
CEC <- NULL

#' @title loop()
#' Start loop of the algorithm where learning is done
#'
#' @docType methods
#'
#' @param c CEC model object
loop.cec <- NULL

#' @title entropy()
#' Print result entropy of clustering found
#' 
#' @docType methods
#'
#' @param c CEC model object
entropy.cec <- NULL

#' @title y()
#' Print labels assigned
#' #' 
#' @docType methods
#'
#' @param c CEC model object
y.cec <- NULL

#' @title centers()
#' Print centers of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
centers.cec <- NULL

#' @title cov()
#' Print covariances of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
cov.cec <- NULL

#' @title predictCluster()
#' For given point predict cluster where it belongs
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictCluster.cec <- NULL

#' @title predictClusters()
#' For given point print propabilities of belonging to each cluster
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictClusters.cec <- NULL

#' @title log.ncluster.cec()
#' Print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object
log.ncluster.cec <- NULL

#' @title log.energy.cec()
#' Print energy recorded that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object
log.energy.cec <- NULL

#' @title log.iters.cec()
#' Print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object
log.iters.cec <- NULL

#' @title nstart()
#' Print number of times CEC algorithm is to be performed. The best outcome
#' is chosen as result.
#'  
#' @docType methods
#'
#' @param c CEC model object
nstart.cec <- NULL

evalqOnLoad({

    CEC <<- function(x = NULL,
                     k = 0,
                     method.type = NULL,
                     method.init = "kmeans++",
                     params.r = 0,
                     params.cov = NULL,
                     params.centroids = NULL,
                     params.mix = NULL,
                     params.function = ""
                     control.nstart = 1,
                     control.eps = 1e-4,
                     control.itmax = 1,
                     log.energy = FALSE,
                     log.ncluster= FALSE,
                     log.iters = FALSE){
      
      # check for errors
      
      if (is.null(x))
        Rcpp::stop("Dataset is required!");
      
      if (k <= 0)
        Rcpp::stop("Number of clusters should be a positive integer!");
      
      if (control.nstart <= 0)
        Rcpp::stop("Number of starts should be a positive integer!");
      
      if (control.eps > 1.0 / k)
        Rcpp::stop("killThreshold " + contorl.eps + " is too high");  
      
      if (control.itmax <= 0)
        Rcpp::stop("Max number of iterations should be a positive integer!");
      
        config <- new(cecConfiguration)
        config$setX(x)
        config$setK(k)
        config$setMethodType(method.type)
        config$setMethodInit(method.init)
        config$setR(params.r)
        config$setCov(params.cov)
        config$setCentroid(params.centroids)
        config$setMix(params.mix)
        config$setFunction(params.function)
        config$setNstart(control.nstart)
        config$setEps(control.eps)
        config$setItax(control.itmax)
        config$setLogEnergy(log.energy)
        config$setNCluster(log.ncluster)
        config$setIters(log.iters)
      
        cecClient <- new(cecClient, config)
        cec <- cecClient$findBestCEC()
    }

    loop.cec <<- function(c) {
        c$loop()
    }

    entropy.cec <<- function(c) {
        c$entropy()
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
    setGeneric("y", function(c) standardGeneric("y"))
    setGeneric("centers", function(c) standardGeneric("centers"))
    setGeneric("cov", function(c) standardGeneric("cov"))
    setGeneric("predictCluster", function(c, ...) standardGeneric("predictCluster"))
    setGeneric("predictClusters", function(c, ...) standardGeneric("predictClusters"))
    setGeneric("log.ncluster", function(c) standardGeneric("log.ncluster"))
    setGeneric("log.energy", function(c) standardGeneric("log.energy"))
    setGeneric("log.iters", function(c) standardGeneric("log.iters"))
    setGeneric("nstart", function(c) standardGeneric("nstart"))
    
    setMethod("loop", "Rcpp_cec", loop.cec)
    setMethod("entropy", "Rcpp_cec", entropy.cec)
    setMethod("y", "Rcpp_cec", y.cec)
    setMethod("centers", "Rcpp_cec", centers.cec)
    setMethod("cov", "Rcpp_cec", cov.cec)
    setMethod("predictCluster", "Rcpp_cec", predictCluster.cec)    
    setMethod("predictClusters", "Rcpp_cec", predictClusters.cec)
    setMethod("log.ncluster", "Rcpp_cec", log.ncluster.cec)
    setMethod("log.energy", "Rcpp_cec", log.energy.cec)
    setMethod("log.iters", "Rcpp_cec", log.iters.cec)
    setMethod("nstart", "Rcpp_cec", nstart.cec)
})
