breast_cancer.path <- system.file("data_sets","svm","breast_cancer.data", package="gmum.r")
twoe_data.path <- system.file("data_sets","svm","2e.data", package="gmum.r")
circles_data.path <- system.file("data_sets","svm","circles.data", package="gmum.r")

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
      
      index = as.numeric( index_value[1] ) + 1		# +1 because label goes first
      value = as.numeric( index_value[2] )
      
      yx[i, index] = value
    }
  }
  
  return( data.frame(yx) )
}


svm.dataset.breast_cancer <- function() {
  bc = read.libsvm( breast_cancer.path, 10 )
  bc$X2 = NULL
  return(bc)
}

svm.dataset.2e <- function() {
  te = read.table(twoe_data.path, quote="\"")
  return(te)
}

svm.dataset.circles <- function() {
  te = read.table(circles_data.path, quote="\"")
  return(te)
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





