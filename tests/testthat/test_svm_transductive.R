# 1. Tests if we handle factors as e1071
# 2. Tests if we get same results on induction task as svm_learn (hard coded)
# 3. Tests if transductive learning works on dataset from svmlight website
library(testthat)
library(e1071)
library(caret)
library(gmum.r)


test_that("Transduction and factors work as in respectively svmlight and libsvm", {
  data(svm.transduction)
  
  train.transduction <- svm.transduction$tr
  train.induction <- svm.transduction$ind
  test <-svm.transduction$test
  test$x <- train.transduction$x[11:610, ]
  
  
  # Checks if we handle labels same way!
  library(e1071)
  svm.induction.e <- svm(x=train.induction$x, y=train.induction$y, kernel='linear')
  svm.induction.e.pred <- predict(svm.induction.e, test$x[,1:9253])
  
  svm.induction <- SVM(x=train.induction$x, y=train.induction$y, core="svmlight", verbosity=1)
  svm.induction.pred <- predict(svm.induction, test$x[,1:9253])
  
  
  # Now revers labels 
  train.induction$y <- as.factor(as.numeric(train.induction$y))
  train.transduction$y <- as.numeric(train.transduction$y)
  train.transduction$y[train.transduction$y == 1] = 0
  train.transduction$y  <- as.factor(train.transduction$y)
  test$y <- (as.numeric(test$y))
  test$y[test$y == 1] = 3
  test$y[test$y == 2] = 1
  test$y[test$y == 3] = 2
  test$y <- as.factor(test$y)
  
  # Both e1071 and gmum.r fails on incorrect factor
  expect_that(svm.accuracy(svm.induction.e.pred, test$y) < 0.2 && abs(svm.accuracy(svm.induction.e.pred, test$y) - svm.accuracy(svm.induction.pred, test$y)) < 1e-2, is_true())
  
  # Now reverse labelsagain
  train.induction$y <- as.factor(as.numeric(train.induction$y))
  train.transduction$y <- as.numeric(train.transduction$y)
  train.transduction$y[train.transduction$y == 1] = 0
  train.transduction$y  <- as.factor(train.transduction$y)
  test$y <- (as.numeric(test$y))
  test$y[test$y == 1] = 3
  test$y[test$y == 2] = 1
  test$y[test$y == 3] = 2
  test$y <- as.factor(test$y)
  
  svm.transduction <- SVM(x=train.transduction$x, y=train.transduction$y, 
                          core="svmlight",
                          transductive.learning=TRUE)
  svm.transduction.pred <- predict(svm.transduction, test$x)
  
  
  svm.induction <- SVM(x=train.induction$x, y=train.induction$y, core="svmlight", verbosity=1)
  svm.induction.pred <- predict(svm.induction, test$x[,1:9253])
  
  # This is exactly what svm_learn and svm_classify return on this dataset
  # If this fails, we probably changed some defaults in SVMLight - which would be a bug probably
  expect_that(abs(svm.accuracy(svm.induction.pred, test$y) - 0.8433) < 1e-2, is_true())
  expect_that(abs(svm.accuracy(svm.transduction.pred, test$y) - 0.96) < 1e-2, is_true())
  
  # Check if we pass posratio correctly
  svm.transduction <- SVM(x=train.transduction$x, y=train.transduction$y, 
                          core="svmlight",
                          transductive.learning=TRUE,
                          transductive.posratio=0.001)
  svm.transduction.pred <- predict(svm.transduction, test$x)
  
  expect_that(abs(svm.accuracy(svm.transduction.pred, test$y)
                  - 0.5) < 1e-2, is_true())
})

test_that("Transduction improves score", {
  library(SparseM)
  library(e1071)
  library(caret)
  library(gmum.r)
  set.seed(777)
  
  x <- read.matrix.csr(system.file("data_sets", "svm", "dexter_train.data", mustWork=TRUE, package="gmum.r"))
  y <- as.factor(unlist(read.table(system.file("data_sets", "svm", "dexter_train.labels", mustWork=TRUE, package="gmum.r"))))
  
  X <- SparseM::as.matrix(x)
  Y <- as.numeric(y)
  
  i <- createDataPartition(Y, times=1, p=0.8, list=FALSE)
  #A lot of transductive labels to make sure the effect is visible
  K=as.integer(0.9*length(i)) 
  
  X.test <- X[-i,]
  Y.test <- Y[-i]
  X.train <- X[i,]
  Y.train <- Y[i]
  X.train.tr1 <- X[i,]
  Y.train.tr1 <- Y[i]
  trans1 <- sample(1:length(i), K, replace=FALSE)
  trans2 <- sample(1:length(i), K, replace=FALSE)
  Y.train.tr1[trans1] = 0
  
  NmK <- length(i) - K
  
  
  svm.1 <- SVM(X.train, Y.train, core="svmlight", kernel="linear")
  svm.2a <- SVM(X.train.tr1, Y.train.tr1, 
                transductive.learning = TRUE, core="svmlight", kernel="linear", verbosity=1)
  sample1 <- sample(1:nrow(X.train), NmK, replace=FALSE)
  svm.3a <- SVM(X.train[sample1,],as.factor(Y.train[sample1]), core="svmlight", kernel="linear")
  
  
  svm.1.acc <- svm.accuracy(predict(svm.1, X.test), Y.test)
  svm.2a.acc <- svm.accuracy(predict(svm.2a, X.test), Y.test)
  svm.3a.acc <- svm.accuracy(predict(svm.3a, X.test), Y.test)
  
  expect_that(svm.1.acc > svm.2a.acc && svm.2a.acc > svm.3a.acc, is_true())
  
})