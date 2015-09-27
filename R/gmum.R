#' @importFrom grDevices rainbow
#' @importFrom graphics hist pairs par plot plot.new points title
#' @importFrom stats na.omit prcomp predict rnorm runif update
#' @importFrom utils data read.csv
#' @importFrom Matrix Matrix
NULL

# Lazy loading to allow for discovery of all files
evalqOnLoad( {
  # Autocompletion override
  autocompl <- function(x, pattern="") {
    targets <- c(asNamespace("Rcpp")$complete(x), x[['.staticFields']])
    grep(pattern, targets, value = TRUE)[! (substr(grep(pattern, targets, value = TRUE),1,1)==".")]
  }
  
  `.DollarNames.Rcpp_C++Object` <<- autocompl
  .DollarNames.Rcpp_SVMClient <<- autocompl
  .DollarNames.Rcpp_GNGServer <<- autocompl
  .DollarNames.Rcpp_CecModel <<- autocompl
  
  # Workaround RStudio bug
  if(exists(".rs.getAnywhere") && !exists(".rs.getAnywhere.original")) {
    .rs.getAnywhere.original <<- .rs.getAnywhere
    .rs.getAnywhere <<- function(a, envir=.GlobalEnv){ .rs.getAnywhere.original(a, .GlobalEnv) }
  }
})
