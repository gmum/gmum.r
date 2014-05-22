# Always clear workspace before installing new version of module
# Because Module function is caching 
rm(list=ls(all=TRUE))

library(Rcpp)
# library(RcppArmadillo)
library(devtools)
library(inline)

# Clean and install (load_all could have caused some problems - not sure here)
devtools::clean_dll(".")
devtools::install(".")

# Install module
x = library("gmum.r")
m<-Module("svm_wrapper", PACKAGE=x)
con = m$SVMConfiguration
cli = m$SVMClient

config = new(con)
client = new(cli)

x = matrix( 
      c(2, 4, 3, 1, 5, 7), 
      nrow=3, 
      ncol=2) 

y = c(1,2)

config$data = x
config$createParams( "poly", "libsvm", "norm", 2, 0.1, 0.0 )
config
# client$run( config )


