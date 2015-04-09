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
  }
  
  setMethod("summary", "Rcpp_CecModel", summary.cec)
  setMethod("show", "Rcpp_CecModel", summary.cec)
})