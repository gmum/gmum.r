library(gmum.r)

x <- read.matrix.csr(system.file("inst", "data_sets", "svm", "mushrooms.x", mustWork=TRUE, package="gmum.r"))
y <- read.table(system.file("inst", "data_sets", "svm", "mushrooms.y", mustWork=TRUE, package="gmum.r"))
y <- as.vector(unlist(y))

libs <- c("svmlight", "libsvm")
kernels <- c("linear", "poly", "rbf")

for (lib_i in libs) {
  for (kernel_i in kernels) {
    
    train_start <- Sys.time()
    svm <- SVM(x=x, y=y, lib=lib_i, kernel=kernel_i, C=1, gamma=1, verbosity=0)
    gmum_train_time <- Sys.time() - train_start
    
    test_time <- Sys.time()
    gmum_pred <- predict(svm, x)
    gmum_test_time <- Sys.time() - test_time
    
    gmum_acc <- svm.accuracy(prediction=gmum_pred, target=y)
    
    if (kernel_i == "rbf") kernel_i = "radial"
    train_time <- Sys.time() 
    e_svm <- e1071::svm(x=x, y=y, type='C-classification', kernel=kernel_i, cost=1, gamma=1, scale=FALSE, fitted=FALSE)
    e_train_time <- Sys.time() - train_time
    
    test_time <- Sys.time()
    e_pred <- as.integer(predict(e_svm, x))
    e_test_time <- Sys.time() - test_time
    
    e_acc <- svm.accuracy(e_pred, y)
    
    print(sprintf("gmum.r %s %s acc: %.3f", lib_i, kernel_i, gmum_acc))
    print(sprintf("e1071 %s acc: %.3f", kernel_i, e_acc))
    print("---")
    print(sprintf("gmum.r %s %s train time: %.2f", lib_i, kernel_i, gmum_train_time))
    print(sprintf("e1071 %s train time: %.2f", kernel_i, e_train_time))
    print("---")
    print(sprintf("gmum.r %s %s test time: %.2f", lib_i, kernel_i, gmum_test_time))
    print(sprintf("e1071 %s test time: %.2f", kernel_i, e_test_time))
    print("---")
    print(sprintf("gmum.r %s %s nSV: %i", lib_i, kernel_i, svm$get_number_sv()))
    print(sprintf("e1071 %s nSV: %i", kernel_i, nrow(e_svm$SV)))
    print("---")
    print("======================================")
    
  }
}