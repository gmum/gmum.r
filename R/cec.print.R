loadModule("cec", TRUE)

#' Print CEC
#'
#' @title print
#' 
#' @description Print basic information about clusters found.
#' Presents a structure of the cec results object (clusters found)
#'
#' @docType methods
#'
#' @param x CEC object model.
#' 
#' @export
#' 
#' @usage print(cec)
print.cec <- NULL

evalqOnLoad({
  
  print.cec <<- function(x) {
    print(sprintf("Cec clustering, %d clusters with %f entropy",
                  length(x$centers()), x$entropy()))
    print("Centers : ")
    print(x$centers())
    print("Covariances : ")
    print(x$cov())
  }
  
  setMethod("print", "Rcpp_cecModel", print.cec)
  
})
