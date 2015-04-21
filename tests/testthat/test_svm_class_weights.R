library(testthat)
library(e1071)
library(SparseM)

test_that('class.weights works for libsvm', {
  liver <- read.matrix.csr(system.file("data_sets", "svm", "liver-disorders", mustWork=TRUE, package="gmum.r"))
  x <- as.matrix(liver$x)
  # Ugly change of level 
  y <- liver$y
  y <- as.matrix(y)
  y[y==1] <- 3
  y <- as.factor(y)
  
  i <- createDataPartition(y, times=1, list=FALSE)
  
  wsvm <- SVM(x[i, ], y[i], lib="libsvm", kernel="linear", 
              C=1, class.weights=list("3"=1.0,"2"=3.0))
  pred.1 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
  
  wsvm <- SVM(x[i, ], y[i], lib="libsvm", kernel="linear", 
              C=1, class.weights=list("2"=3.0,"3"=1.0))
  pred.2 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
  
  # We have good names in class.weights support
  expect_that(pred.1 == pred.2, is_true())
  
  # Ugly code for prediction using e1071
  data <- as.data.frame(x)
  data$y = as.factor(y)
  esvm <- e1071::svm(y ~ .,type='C-classification',scale=FALSE, kernel='linear', C=1,
                     data=data[i,], class.weights = c("3"=1., "2"=3.))
  pred.esvm <- svm.accuracy(predict(esvm, data[-i,]), data[-i,]$y)
  
  # Prediction e1071 == libsvm from gmum.R
  expect_that(pred.1 == pred.2 && pred.1 == pred.esvm, is_true())
})
  

test_that('class.weights doesnt crash for svmlight', {
  liver <- read.matrix.csr(system.file("data_sets", "svm", "liver-disorders", mustWork=TRUE, package="gmum.r"))
  x <- as.matrix(liver$x)
  # Ugly change of level 
  y <- liver$y
  y <- as.matrix(y)
  y[y==1] <- 3
  y <- as.factor(y)
  
  i <- createDataPartition(y, times=1, list=FALSE)
  
  wsvm <- SVM(x[i, ], y[i], lib="svmlight", kernel="linear", 
              C=1, class.weights=list("3"=1.0,"2"=3.0))
  pred.1 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
  
  wsvm <- SVM(x[i, ], y[i], lib="svmlight", kernel="linear", 
              C=1, class.weights=list("2"=3.0,"3"=1.0))
  pred.2 <- svm.accuracy(predict(wsvm, x[-i, ]), y[-i])
  
  # We have good names in class.weights support
  expect_that(pred.1 == pred.2, is_true())
})


