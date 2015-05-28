library(testthat)
library('gmum.r')

data(svm_breast_cancer_dataset)

test_that('command line svm.options are being handled for core="libsvm"', {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  x <- ds[,-1]
  y <- ds[,1]
  formula <- X1 ~ .
  
  # Prediction has lower accuracy (0.6500732) when LibSVM learns with C=0.0001
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", svm.options="-c 0.0001"); 
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=y)
  expect_that(acc < 0.95, is_true())

  # Prediction has good accuracy (0.9721816) when LibSVM learns with C=100
  svm <- SVM(formula, ds, core="libsvm", kernel="linear", svm.options="-c 100"); 
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=y)
  expect_that(acc > 0.95, is_true())
})

test_that('command line svm.options are being handled for core="svmlight"', {
  
  data(svm_breast_cancer_dataset)
  ds <- svm.breastcancer.dataset
  ds[,'X1'] <- as.factor(ds[,'X1'])
  x <- ds[,-1]
  y <- ds[,1]
  formula <- X1 ~ .
  
  # Prediction has lower accuracy (0.6500732) when SVMLight learns with C=0.0001
  svm <- SVM(formula, ds, core="svmlight", kernel="linear", svm.options="-c 0.0001"); 
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=y)
  expect_that(acc < 0.95, is_true())

  # Prediction has good accuracy (0.9721816) when SVMLight learns with C=100
  svm <- SVM(formula, ds, core="svmlight", kernel="linear", svm.options="-c 100"); 
  pred <- predict(svm, x)
  acc <- svm.accuracy(prediction=pred, target=y)
  expect_that(acc > 0.95, is_true())
})



