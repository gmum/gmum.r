# Lazy loading to allow for discovery of all files
evalqOnLoad( {
  # Autocompletion fix
  .GlobalEnv$`.DollarNames.C++Object` <- function( x, pattern ){
    targets <- c(asNamespace("Rcpp")$complete(x), x[['.staticFields']])
    grep(pattern, targets, value = TRUE)[! (substr(grep(pattern, targets, value = TRUE),1,1)==".")]
  }
})
