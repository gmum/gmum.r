library(testthat)
library(gmum.r)

test_that("sparse matrices work", {
  x <- read.matrix.csr(system.file("dev", "datasets", "svm", "dexter_train.data", mustWork=TRUE, package="gmum.r"))
  y <- as.factor(unlist(read.table(system.file("dev", "datasets", "svm", "dexter_train.labels", mustWork=TRUE, package="gmum.r"))))

    libs <- c("svmlight", "libsvm")
    kernels <- c("linear", "poly", "rbf", "sigmoid")

    for (lib_i in libs) {
        for (kernel_i in kernels) {
            print("")
            print(sprintf("Kernel %s", kernel_i))
            print(sprintf("Testing %s", lib_i))

            print("Learning...")
            svm <- SVM(x=x, y=y, core=lib_i, kernel=kernel_i, C=1, verbosity=0)

            print("Prediction...")
            pred_start_time <- Sys.time()
            pred <- predict(svm, x)
            pred_time <- Sys.time() - pred_start_time
            print(sprintf("Prediction %f", pred_time))

            acc <- svm.accuracy(prediction=pred, target=y)
            print(sprintf("Sparse acc, %s prediction: %f", lib_i, acc))
        }
  }

})


