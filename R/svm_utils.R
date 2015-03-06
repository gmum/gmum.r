# farm_ads.path <- system.file("data_sets","svm", "", package="gmum.r")
# http://archive.ics.uci.edu/ml/machine-learning-databases/00218/farm-data.zip

urls_data.path <- system.file("data_sets","svm", "Day0.svm", package="gmum.r")
# http://archive.ics.uci.edu/ml/machine-learning-databases/url/url_svmlight.tar.gz

svm.data.root <- system.file("data_sets", "svm", package="gmum.r")
colon_cancer.url <- "http://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/binary/colon-cancer.bz2" 
colon_cancer.filename <- "colon-cancer"

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
  colon_cancer.path <- svm.download.dataset(colon_cancer.url, colon_cancer.filename)
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

#right now is working only wiz bzip2
svm.download.dataset <- function(url, filename) {
	destfile <- file.path(svm.data.root,filename)
	if(!file.exists(destfile)) {
		filename_download <- basename(url) 
		ext <- strsplit(filename_download, "\\.")[[1]][2]
		ext <- switch(ext,
		       bz2 = "bzip2")
		destfile_download <- file.path(svm.data.root,filename_download)
		download.file(url,destfile=destfile_download)	
		destfile_con <- file(destfile, "w")
		destfile_download_con <- bzfile(destfile_download)
		writeLines(readLines(destfile_download_con, warn=FALSE), destfile_con) 
		unlink(destfile_download)
		close(destfile_con)
		close(destfile_download_con)
	}
	destfile
}

