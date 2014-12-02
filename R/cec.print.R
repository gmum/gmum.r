loadModule("cec", TRUE)

#' Print
#'
#' @title print
#' 
#' @description Print basic information about clusters found.
#' Presents a structure of the cec results object (clusters found)
#'
#' @docType methods
#'
#' @param x CEC object model
#'
#' @usage print(cec)
print.cec <- NULL

evalqOnLoad({

    print.cec <<- function(x) {
        print(sprintf("Cec clustring, %d clusters with %f entropy",
                      length(x$centers()), x$entropy()))
        print("Centers : ")
        print(x$centers())
        print("Covariances : ")
        print(x$cov())
    }

    setMethod("print","Rcpp_cecModel", print.cec)

})
