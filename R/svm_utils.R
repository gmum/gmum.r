# farm_ads.path <- system.file("data_sets","svm", "", package="gmum.r")
# http://archive.ics.uci.edu/ml/machine-learning-databases/00218/farm-data.zip

urls_data.path <- system.file("data_sets","svm", "Day0.svm", package="gmum.r")
# http://archive.ics.uci.edu/ml/machine-learning-databases/url/url_svmlight.tar.gz

svm.data.root <- system.file("data_sets", "svm", package="gmum.r")

colon_cancer.filename <- "colon-cancer"
colon_cancer.path <- file.path(svm.data.root,colon_cancer.filename)

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
      
      index = as.numeric( index_value[1] ) + 1  	# +1 because label goes first
      value = as.numeric( index_value[2] )
      
      yx[i, index] = value
    }
  }
  
  return( data.frame(yx) )
}

# svm.dataset.urls_data <- function()  {
#   bc <- read.libsvm( urls_data.path, 3231961 )
#   return(bc)
# }

# svm.dataset.farm_ads <- function()  {
#   bc <- read.libsvm( farm_ads.path, 54877 )
#   return(bc)
# }

svm.dataset.colon_cancer <- function()  {
  bc <- read.libsvm( colon_cancer.path,  2000 )
  return(bc)
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
  
  diff <- target-prediction
  acc <- sum(diff == 0) / len
  return(acc) 
}



