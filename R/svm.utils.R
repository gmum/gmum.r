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

    diff = as.numeric(target) - as.numeric(prediction)
    acc <- sum(diff == 0) / length(target)
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

scale.data.frame <-
  function(x, center = TRUE, scale = TRUE)
  {
    i <- sapply(x, is.numeric)
    if (ncol(x[, i, drop = FALSE])) {
      x[, i] <- tmp <- scale.default(x[, i, drop = FALSE], na.omit(center), na.omit(scale))
      if(center || !is.logical(center))
        attr(x, "scaled:center")[i] <- attr(tmp, "scaled:center")
      if(scale || !is.logical(scale))
        attr(x, "scaled:scale")[i]  <- attr(tmp, "scaled:scale")
    }
    x
  }

read.matrix.csr <- function(file, fac = TRUE, ncol = NULL) {
  l <- strsplit(readLines(file), "[ ]+")
  
  ## extract y-values, if any
  y <- if (is.na(l[[1]][1]) || length(grep(":",l[[1]][1])))
    NULL
  else
    sapply(l, function(x) x[1])
  
  ## x-values
  rja <- do.call("rbind",
                 lapply(l, function(x)
                   do.call("rbind",
                           strsplit(if (is.null(y)) x else x[-1], ":")
                   )
                 )
  )
  ja <- as.integer(rja[,1])
  ia <- cumsum(c(1, sapply(l, length) - !is.null(y)))
  
  max.ja <- max(ja)
  dimension <- c(length(l), if (is.null(ncol)) max.ja else max(ncol, max.ja))
  x = new(getClass("matrix.csr", where = asNamespace("SparseM")),
          ra = as.numeric(rja[,2]), ja = ja,
          ia = as.integer(ia), dimension = as.integer(dimension))
  if (length(y))
    list(x = x, y = if (fac) as.factor(y) else as.numeric(y))
  else x
}
