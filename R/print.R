loadModule("cec", TRUE)

#' print()
#'
#' @title print()
#' print basic information about clusters found
#'
#' @docType methods
#'
#' @param x CEC object model
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

    setMethod("print","Rcpp_cec", print.cec)

})
