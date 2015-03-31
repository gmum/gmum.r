#' Summary CEC
#' 
#' @title summary
#' 
#' @description Print detailed information about CEC model object
#'
#' @docType methods
#'
#' @param object CEC model object.
#' 
#' @usage summary(cec)
summary.cec <- NULL

evalqOnLoad({  
  summary.cec <<- function(object) {
    print.cec(object)
    names = c("x", "k", "clustering", "method.type", "method.init",
              "params.r", "params.covMatrix", "control.nstart", "control.eps",
              "control.itmax", "log.energy", "log.ncluster", "log.iters")
    datalength = length(object$clustering())
    nrClusters = length(object$centers())
    bestNumberOfSteps = length(object$log.energy())
    Length = c(datalength, nrClusters, nrClusters, nrClusters, 1,
               nrClusters, nrClusters, 1, 1, bestNumberOfSteps, rep(1, 3))
    Class = rep("-none-",length(names))
    Mode = c(rep("numeric", 3), "character", "character", rep("numeric", 5), rep("logical", 3))
    print(data.frame(names, Length, Class, Mode), row.names = FALSE)
  }
  
  setMethod("summary", "Rcpp_CecModel", summary.cec)
  setMethod("show", "Rcpp_CecModel", summary.cec)
})