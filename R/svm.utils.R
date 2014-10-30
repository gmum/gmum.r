breast_cancer.path = "./data/svm/breast_cancer.data"
twoe_data.path = "./data/svm/2e.data"

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
  
  return( yx )
}


svm.dataset.breast_cancer <- function() {
  bc = read.libsvm( breast_cancer.path, 10 )
  return(data.frame(bc))
}

svm.dataset.2e <- function() {
  te = read.table(twoe_data.path, quote="\"")
  return(te)
}

svm.dataset.xor <- function() {
  matrix( 
    c(0,1,0,1,0,0,1,1,0,1,1,0),
    ncol=3,
    nrow=4)
}

check.trained <- function(object) {
  if ( object$trained ) {
    warning("You are changing parameters of a trained model, remember to train it again before prediction.")
  }
}


accuracy.svm <- function(object=NULL, y=NULL, t) {
  if (is.null(object) && !is.null(y) && t != FALSE ) {
    if ( length(t) != length(y)) {
      stop("Result's and target's length dont match!")
    }
    len <- length(t)
    
    diff = t-y
    acc <- sum(diff == 0) / len
    return(acc)
  } 
  else if (!is.null(object) && is.null(y) &&  t != FALSE) {
    x <- object$getX()
    object$train()
    object$predict(x)
    y <- object$getPrediction()
    
    if ( length(t) != length(y)) {
      stop("Result's and target's length dont match!")
    }
    len <- length(t)
    
    diff = t-y
    acc <- sum(diff == 0) / len
    return(acc)
  }
  else {
    stop("You need to provide either model and target, or result and target")
  }
  
}





