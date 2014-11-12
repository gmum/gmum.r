loadModule("cec", TRUE)

#' Summary CEC
#' 
#' @title summary
#' 
#' @description print detailed information about CEC model object
#'
#' @docType methods
#'
#' @param object CEC model object
summary.cec <- NULL

evalqOnLoad({
    
    summary.cec<<- function(object) {
        print.cec(object)
        names = c("x", "k", "clustering", "method.type", "method.init",
            "params.r", "params.cov", "control.nstart", "control.eps",
            "control.itmax", "log.energy", "log.ncluster", "log.iters")
        datalength = length(object$y())
        nrClusters = length(object$centers())
        bestNumberOfSteps = length(object$log.energy())
        Length = c(datalength, nrClusters, nrClusters, nrClusters, 1,
            nrClusters, nrClusters, 1, 1, 1, bestNumberOfSteps,
            bestNumberOfSteps,1)
        Class = rep("-none-",length(names))
        Mode = c("numeric", "numeric", "character", "character", "character", rep("numeric",8))
        print(data.frame(names, Length, Class, Mode), row.names=FALSE)
    }

    setMethod("summary","Rcpp_cec",summary.cec)

})