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
#' @param ... other arguments not used by this method.
#' 
#' @export
#' 
print.cec <- NULL


  
print.cec <- function(x, ...) {
    print(sprintf("CEC clustering; %d clusters with energy = %f",
                  length(x$centers()), x$energy()))
    print("Centers: ")
    print(x$centers())
    print("Covariances: ")
    print(x$covMatrix())
}

evalqOnLoad({
    setMethod("print","Rcpp_CecModel", print.cec)    
})
