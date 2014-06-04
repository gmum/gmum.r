.onLoad <- function(libname, pkgname) {
  library(Rcpp)
  library(RcppArmadillo)
  library(devtools)
  library(inline)
  loadModule("svm_wrapper", TRUE)
}
