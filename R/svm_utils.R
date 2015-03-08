svm.data.root <- system.file("data_sets", "svm", package="gmum.r")
colon_cancer.filename <- "colon-cancer"
colon_cancer.path <- file.path(svm.data.root, colon_cancer.filename)

svm.dataset.breast_cancer <- function() {
  data(svm_breast_cancer_dataset)
  return (svm.breastcancer.dataset)
}

svm.dataset.2e <- function() {
  data(svm_two_ellipsoids_dataset)
  return(svm.twoellipsoids.dataset)
}

svm.dataset.circles <- function() {
  data(svm_two_circles_dataset)
  return(svm.twocircles.dataset)
}

svm.dataset.xor <- function() {
  matrix( 
    c(0,1,0,1,0,0,1,1,0,1,1,0),
    ncol=3,
    nrow=4,
    dimnames=list(c(),c("x","y","t")))
}

svm.accuracy <- function(prediction, target) {
  if ( length(target) != length(prediction)) {
    stop("Prediction's and target's length don't match!")
  }
  len <- length(target)
  
  diff = target-prediction
  acc <- sum(diff == 0) / len
  return(acc) 
}

read.libsvm <- function(filename, dimensionality) {
  
  content = readLines( filename )
  num_lines = length( content )
  yx = matrix( 0, num_lines, dimensionality + 1 )
  
  # loop over lines
  for ( i in 1:num_lines ) {
    
    # split by spaces
    line = as.vector( strsplit( content[i], ' ' )[[1]])
    
    # save label
    yx[i,1] = as.numeric( line[[1]] )
    
    # loop over values
    for ( j in 2:length( line )) {
      
      # split by colon
      index_value = strsplit( line[j], ':' )[[1]]
      
      index = as.numeric( index_value[1] ) + 1    # +1 because label goes first
      value = as.numeric( index_value[2] )
      
      yx[i, index] = value
    }
  }
  
  return( data.frame(yx, stringsAsFactors=FALSE) )
}