#' Summary CEC
#' @export summary.Rcpp_CecModel
#' @rdname summary.cec
#' @method summary Rcpp_CecModel 
#' 
#' @title summary
#' 
#' @description Print detailed information about CEC model object
#'
#' @docType methods
#'
#' @param object CEC model object.
#' @param ... other arguments not used by this method.
#' 
summary.Rcpp_CecModel <- function(object, ...) {
  print(object)
  
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

show.Rcpp_CecModel <- function(object) {
    summary(object)
}

isParameterOn <- function(x) {
  return(length(x) != 0)
}
