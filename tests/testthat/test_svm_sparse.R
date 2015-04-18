library(testthat)
library(gmum.r)

test_that("sparse matrices work", {
  x <- read.matrix.csr(system.file("inst", "data_sets", "svm", "dexter_train.data", mustWork=TRUE, package="gmum.r"))
  y <- as.vector(unlist(read.table(system.file("inst", "data_sets", "svm", "dexter_train.labels", mustWork=TRUE, package="gmum.r"))))

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


