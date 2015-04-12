svm.data.root <- system.file("data_sets", "svm", package="gmum.r")
svm.colon_cancer.path <- file.path(svm.data.root, "colon-cancer")

svm.lib.libsvm <- "libsvm"
svm.lib.svmlight <- "svmlight"

svm.prep.2e <- "2e"
svm.prep.none <- "none"

svm.kernel.linear <- "linear"
svm.kernel.poly <- "poly"
svm.kernel.rbf <- "rbf"
svm.kernel.sigmoid <- "sigmoid"

svm.plot.contour <- "contour"
svm.plot.pca <- "pca"

svm.dataset.breast_cancer <- function() {
  data(svm_breast_cancer_dataset)
  return (svm.breastcancer.dataset)
}

svm.dataset.colon_cancer <- function() {
  bc <- read.libsvm(svm.colon_cancer.path, 2000)
  return(bc)
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

multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  require(grid)
  
  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)
  
  numPlots = length(plots)
  
  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  
  if (numPlots==1) {
    print(plots[[1]])
    
  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    
    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}



