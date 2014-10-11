loadModule("cec", TRUE)

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
#' start loop of the algorithm where learning is done
#'
#' @docType methods
#'
#' @param c CEC model object
loop.cec <- NULL

#' @title entropy()
#' print result entropy of clustering found
#' 
#' @docType methods
#'
#' @param c CEC model object
entropy.cec <- NULL

#' @title y()
#' print labels assigned
#' #' 
#' @docType methods
#'
#' @param c CEC model object
y.cec <- NULL

#' @title centers()
#' print centers of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
centers.cec <- NULL

#' @title cov()
#' print covariances of clusters
#'
#' @docType methods
#'
#' @param c CEC model object
cov.cec <- NULL

#' @title predictCluster()
#' for given point predict cluster where it belongs
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictCluster.cec <- NULL

#' @title predictClusters()
#' for given point print propabilities of belonging to each cluster
#' 
#' @docType methods
#'
#' @param c CEC model object
#' @param vec given point
predictClusters.cec <- NULL

#' @title log.ncluster.cec()
#' print number of clusters that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#' 
#' @docType methods
#'
#' @param c CEC model object
log.ncluster.cec <- NULL

#' @title log.energy.cec()
#' print energy recorded that has been recorded at each stage of learning.
#' Data is recorded only if you has chosen to when you created CEC model object.
#'  
#' @docType methods
#'
#' @param c CEC model object
log.energy.cec <- NULL

#' @title log.iters.cec()
#' print how many iterations it took to learn CEC model
#'  
#' @docType methods
#'
#' @param c CEC model object
log.iters.cec <- NULL

#' @title nstart()
#' print number of times CEC algorithm is to be performed. The best outcome
#' is chosen as result.
#'  
#' @docType methods
#'
#' @param c CEC model object
nstart.cec <- NULL

evalqOnLoad({

    CEC <<- function(args){
        new(cec, args)
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
    setMethod("loop", "Rcpp_cec", loop.cec)
    
    setGeneric("entropy", function(c) standardGeneric("entropy"))
    setMethod("entropy", "Rcpp_cec", entropy.cec)

    setGeneric("y", function(c) standardGeneric("y"))
    setMethod("y", "Rcpp_cec", y.cec)

    setGeneric("centers", function(c) standardGeneric("centers"))
    setMethod("centers", "Rcpp_cec", centers.cec)

    setGeneric("cov", function(c) standardGeneric("cov"))
    setMethod("cov", "Rcpp_cec", cov.cec)

    setGeneric("predictCluster", function(c, ...) standardGeneric("predictCluster"))
    setMethod("predictCluster", "Rcpp_cec", predictCluster.cec)

    setGeneric("predictClusters", function(c, ...) standardGeneric("predictClusters"))
    setMethod("predictClusters", "Rcpp_cec", predictClusters.cec)

    setGeneric("log.ncluster", function(c) standardGeneric("log.ncluster"))
    setMethod("log.ncluster", "Rcpp_cec", log.ncluster.cec)

    setGeneric("log.energy", function(c) standardGeneric("log.energy"))
    setMethod("log.energy", "Rcpp_cec", log.energy.cec)

    setGeneric("log.iters", function(c) standardGeneric("log.iters"))
    setMethod("log.iters", "Rcpp_cec", log.iters.cec)

    setGeneric("nstart", function(c) standardGeneric("nstart"))
    setMethod("nstart", "Rcpp_cec", nstart.cec)
})
