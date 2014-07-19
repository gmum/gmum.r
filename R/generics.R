loadModule("cec", TRUE)
CEC <- NULL
loop.cec <- NULL
entropy.cec <- NULL
y.cec <- NULL
centers.cec <- NULL
cov.cec <- NULL
predictCluster.cec <- NULL
predictClusters.cec <- NULL
log.ncluster.cec <- NULL
log.energy.cec <- NULL
log.iters.cec <- NULL
nstart.cec <- NULL

evalqOnLoad({

	CEC <<- function(args){
        new(cec, args)
    }

    loop.cec <- function(c) {
        c$loop()
    }

    entropy.cec <- function(c) {
        c$entropy()
    }

    y.cec <- function(c) {
        c$y()
    }

    centers.cec <- function(c) {
        c$centers()
    }

    cov.cec <- function(c) {
        c$cov()
    }

    predictCluster.cec <- function(c, vec) {
        c$predict(vec)
    }

    predictClusters.cec <- function(c, vec) {
        c$predict(vec,TRUE)
    }

    log.ncluster.cec <- function(c) {
        c$log.ncluster()
    }

    log.energy.cec <- function(c) {
        c$log.energy()
    }

    log.iters.cec <- function(c) {
        c$log.iters()
    }

    nstart.cec <- function(c) {
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
