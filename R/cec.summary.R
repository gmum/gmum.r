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
  
    if(isParameterOn(object$log.iters())){
      print("Iterations: ")
      print(object$log.iters())
    }
    if(isParameterOn(object$log.energy())){
      print("Energy for every iteration: ")
      print(object$log.energy())
    }
    if(isParameterOn(object$log.ncluster())){
      print("Number of clusters for every iteration: ")
      print(object$log.ncluster())
    }
  }
  
  setMethod("summary", "Rcpp_CecModel", summary.cec)
  setMethod("show", "Rcpp_CecModel", summary.cec)
})

isParameterOn <- function(x) {
  return(length(x) != 0)
}