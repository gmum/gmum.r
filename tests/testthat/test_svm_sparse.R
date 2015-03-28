library(testthat)
library(gmum.r)

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


test_that("sparse matrices work", {
  x <- read.matrix.csr(system.file("inst", "data_sets", "svm", "dexter_train.data", mustWork=TRUE, package="gmum.r"))
  y <- read.table(system.file("inst", "data_sets", "svm", "dexter_train.labels", mustWork=TRUE, package="gmum.r"))

  liblist <- c("svmlight", "libsvm")

  for (lib_i in liblist) {

      print(sprintf("Testing %s", lib_i))

      print("Learning...")
      svm <- SVM(x=x, y=y, lib=lib_i, kernel="linear", C=1, verbosity=0)

      print("Prediction...")
      pred <- predict(svm, x)

      target <- svm$getY()
      acc <- svm.accuracy(prediction=pred, target=target)
      print(sprintf("Sparse acc, %s prediction: %f", lib_i, acc))
  }

})


