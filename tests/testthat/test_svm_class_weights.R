library(testthat)
library(e1071)
library(caret)
library(gmum.r)


test_that('Simple test that class.weights works for libsvm and svmlight', {
  library(SparseM)
  
  for(weight_pos in c(1,2,3)){
    for(lib in c("svmlight", "libsvm")){
      liver <- read.matrix.csr(system.file("data_sets", "svm", "liver-disorders", mustWork=TRUE, package="gmum.r"))
      x <- liver$x
      # Ugly change of level 
      y <- liver$y
      y <- as.numeric(y)
      y[y==1] <- 3
      y <- as.factor(y)
      
      i <- as.numeric(createDataPartition(y, times=1, list=FALSE))
      wsvm <- SVM(x[i, ], y[i], lib=lib, kernel="linear", 
                  C=1, class.weights=list("3"=weight_pos,"2"=3.0))
      pred.1 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
      
      wsvm <- SVM(x[i, ], y[i], lib=lib, kernel="linear", 
                  C=1, class.weights=list("2"=3.0,"3"=weight_pos))
      pred.2 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
      # We have good names in class.weights support
      expect_that(pred.1 == pred.2, is_true())
    
      # Compare to e1071 (libsvm)
      esvm <- e1071::svm(x=x[i,], y=y[i],type='C-classification',scale=FALSE, kernel='linear', C=1,
                         class.weights = c("3"=weight_pos, "2"=3.))
      pred.esvm <- svm.accuracy(predict(esvm, x[-i]), y[-i])
      # Prediction e1071 \approx libsvm/svmlight from gmum.R
      # This should be true for small datasets
      expect_that(pred.1 == pred.2 && abs(pred.1 - pred.esvm) < 1e-2, is_true())
    }
  }
})